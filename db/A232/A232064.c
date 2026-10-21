/*
 * A232064  Number of nX7 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or vertically, with no adjacent values
 *          equal.
 * https://oeis.org/A232064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232064
 *
 * a(n) = number of n×7 arrays with entries 0..2 satisfying
 *   - no two horizontally or vertically adjacent cells have the same value
 *   - each 0 has at least one neighbouring 1 (horizontally or vertically)
 *   - each 1 has at least one neighbouring 2 (horizontally or vertically)
 *
 * The implementation uses dynamic programming over rows.  A state
 * consists of a row pattern (a horizontal 7‑tuple with no equal
 * neighbours) and a request mask derived from the previous row:
 *   - bit j = 1 in mask1 indicates that column j contains a 0 that
 *     still needs a 1 in the row below
 *   - bit j = 1 in mask2 indicates that column j contains a 1 that
 *     still needs a 2 in the row below
 * States are encoded as (pattern_index * REQ_MAX + request_index).
 * Transition tables are pre‑computed once and reused for all n.
 *
 * The routine works for any seq_t/idx_t compile‑time types.
 *********************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------- constants ------------------------------------ */
#define COLS          7
#define PATTERN_MAX   192        /* max number of valid row patterns */
#define REQ_MAX       2187       /* 3^7 different request encodings */
#define MAX_COMPAT    192        /* maximum compatible patterns per pattern */

/* ------------- global tables -------------------------------- */
static unsigned char pat_val[PATTERN_MAX][COLS];
static unsigned char pat_val1[PATTERN_MAX];   /* mask of 1s in pattern */
static unsigned char pat_val2[PATTERN_MAX];   /* mask of 2s in pattern */

static unsigned char compat[PATTERN_MAX][MAX_COMPAT];
static unsigned char compat_cnt[PATTERN_MAX];

static unsigned char new_req1[PATTERN_MAX][PATTERN_MAX];
static unsigned char new_req2[PATTERN_MAX][PATTERN_MAX];

static unsigned char req_mask1[REQ_MAX];
static unsigned char req_mask2[REQ_MAX];
static int          req_idx_map[128][128];

static int          pow3[COLS];

static size_t       pattern_cnt;

/* ------------- runtime state -------------------------------- */
static seq_t *dp   = NULL;
static seq_t *ndp  = NULL;
static int   *act   = NULL;       /* indices of non‑zero states in dp   */
static int   *next  = NULL;       /* indices set in ndp during a step   */
static size_t  act_sz   = 0;
static size_t  next_sz  = 0;
static bool    initialized = false;

/* ------------- initialization -------------------------------- */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* powers of three */
    pow3[0] = 1;
    for (int i = 1; i < COLS; ++i) pow3[i] = pow3[i - 1] * 3;

    /* enumerate all striped row patterns (no equal neighbours) */
    pattern_cnt = 0;
    for (int num = 0; num < 2187; ++num) {
        int tmp = num;
        unsigned char v[COLS];
        bool ok = true;
        for (int j = 0; j < COLS; ++j) {
            v[j] = tmp % 3;
            if (j && v[j] == v[j - 1]) { ok = false; break; }
            tmp /= 3;
        }
        if (!ok) continue;
        uint8_t m1 = 0, m2 = 0;
        for (int j = 0; j < COLS; ++j) {
            pat_val[pattern_cnt][j] = v[j];
            if (v[j] == 1) m1 |= 1 << j;
            if (v[j] == 2) m2 |= 1 << j;
        }
        pat_val1[pattern_cnt] = m1;
        pat_val2[pattern_cnt] = m2;
        ++pattern_cnt;
    }

    /* vertical compatibility lists */
    for (int p = 0; p < pattern_cnt; ++p) {
        compat_cnt[p] = 0;
        for (int q = 0; q < pattern_cnt; ++q) {
            bool ok = true;
            for (int j = 0; j < COLS; ++j)
                if (pat_val[p][j] == pat_val[q][j]) { ok = false; break; }
            if (ok) {
                compat[p][compat_cnt[p]] = (unsigned char)q;
                ++compat_cnt[p];
            }
        }
    }

    /* compute new request masks for every compatible pair */
    for (int p = 0; p < pattern_cnt; ++p)
        for (int q = 0; q < pattern_cnt; ++q) {
            unsigned char n1 = 0, n2 = 0;
            for (int j = 0; j < COLS; ++j) {
                unsigned char val = pat_val[q][j];
                if (val == 0) {
                    bool sat = false;
                    if (j && pat_val[q][j - 1] == 1) sat = true;
                    if (j < COLS - 1 && pat_val[q][j + 1] == 1) sat = true;
                    if (pat_val[p][j] == 1) sat = true;
                    if (!sat) n1 |= 1 << j;
                } else if (val == 1) {
                    bool sat = false;
                    if (j && pat_val[q][j - 1] == 2) sat = true;
                    if (j < COLS - 1 && pat_val[q][j + 1] == 2) sat = true;
                    if (pat_val[p][j] == 2) sat = true;
                    if (!sat) n2 |= 1 << j;
                }
            }
            new_req1[p][q] = n1;
            new_req2[p][q] = n2;
        }

    /* map (mask1, mask2) to request index (base‑3 encoding) */
    for (int m1 = 0; m1 < 128; ++m1)
        for (int m2 = 0; m2 < 128; ++m2)
            req_idx_map[m1][m2] = -1;   /* sentinel */

    for (int m1 = 0; m1 < 128; ++m1)
        for (int m2 = 0; m2 < 128; ++m2) {
            if (m1 & m2) continue;  /* cannot have same position set in both */
            int idx = 0;
            for (int j = 0; j < COLS; ++j) {
                int d1 = (m1 >> j) & 1;
                int d2 = (m2 >> j) & 1;
                idx += (d1 + 2 * d2) * pow3[j];
            }
            req_idx_map[m1][m2] = idx;
        }

    /* pre‑compute masks for each request index */
    for (int r = 0; r < REQ_MAX; ++r) {
        int tmp = r;
        uint8_t m1 = 0, m2 = 0;
        for (int j = 0; j < COLS; ++j) {
            int dig = tmp % 3;
            tmp /= 3;
            if (dig == 1) m1 |= 1 << j;
            else if (dig == 2) m2 |= 1 << j;
        }
        req_mask1[r] = m1;
        req_mask2[r] = m2;
    }

    /* allocate DP arrays and helper lists */
    size_t total_states = pattern_cnt * REQ_MAX;
    dp   = (seq_t*)calloc(total_states, sizeof(seq_t));
    ndp  = (seq_t*)calloc(total_states, sizeof(seq_t));
    act  = (int*)malloc(total_states * sizeof(int));
    next = (int*)malloc(total_states * sizeof(int));
}

/* ------------- main routine --------------------------------- */
seq_t a(idx_t n)
{
    init();                       /* ensure tables are ready */

    const size_t total_states = pattern_cnt * REQ_MAX;

    /* clear dp array */
    act_sz = 0;
    for (size_t i = 0; i < total_states; ++i) dp[i] = 0;

    /* first row initialization */
    for (int p = 0; p < pattern_cnt; ++p) {
        uint8_t m1 = 0, m2 = 0;
        for (int j = 0; j < COLS; ++j) {
            unsigned char v = pat_val[p][j];
            if (v == 0) {
                bool sat = false;
                if (j && pat_val[p][j - 1] == 1) sat = true;
                if (j < COLS - 1 && pat_val[p][j + 1] == 1) sat = true;
                if (!sat) m1 |= 1 << j;
            } else if (v == 1) {
                bool sat = false;
                if (j && pat_val[p][j - 1] == 2) sat = true;
                if (j < COLS - 1 && pat_val[p][j + 1] == 2) sat = true;
                if (!sat) m2 |= 1 << j;
            }
        }
        int req_idx = req_idx_map[m1][m2];
        size_t state = (size_t)p * REQ_MAX + req_idx;
        dp[state] += (seq_t)1;
        act[act_sz++] = (int)state;
    }

    /* n = 1 special case */
    if (n == 1) {
        seq_t ans = 0;
        for (int p = 0; p < pattern_cnt; ++p) {
            size_t state = (size_t)p * REQ_MAX;   /* request index 0 */
            ans += dp[state];
        }
        return ans;
    }

    /* iterate remaining rows */
    idx_t steps = n - 1;
    for (idx_t step = 0; step < steps; ++step) {
        next_sz = 0;

        for (size_t ai = 0; ai < act_sz; ++ai) {
            int idx = act[ai];
            seq_t val = dp[idx];
            if (val == 0) continue;

            int p = idx / REQ_MAX;
            int r = idx % REQ_MAX;
            uint8_t r1 = req_mask1[r];
            uint8_t r2 = req_mask2[r];

            for (int ci = 0; ci < compat_cnt[p]; ++ci) {
                int q = compat[p][ci];
                if ((r1 & ~pat_val1[q]) != 0) continue;   /* 0 needs 1 */
                if ((r2 & ~pat_val2[q]) != 0) continue;   /* 1 needs 2 */

                uint8_t new_m1 = new_req1[p][q];
                uint8_t new_m2 = new_req2[p][q];
                int new_r = req_idx_map[new_m1][new_m2];
                size_t new_idx = (size_t)q * REQ_MAX + new_r;

                if (ndp[new_idx] == 0)
                    next[next_sz++] = (int)new_idx;
                ndp[new_idx] += val;
            }
        }

        /* clear old dp entries */
        for (size_t ai = 0; ai < act_sz; ++ai) dp[act[ai]] = 0;

        /* swap dp/ndp and act/next */
        seq_t *tmp_seq = dp; dp = ndp; ndp = tmp_seq;
        int *tmp_int = act; act = next; next = tmp_int;
        act_sz = next_sz; next_sz = 0;
    }

    /* final answer: states with empty request mask */
    seq_t ans = 0;
    for (size_t ai = 0; ai < act_sz; ++ai)
        if ((act[ai] % REQ_MAX) == 0) ans += dp[act[ai]];

    return ans;
}

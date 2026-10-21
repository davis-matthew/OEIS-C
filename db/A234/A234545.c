/*
 * A234545  Number of (n+1) X (3+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts (n+1)×4 arrays with entries 0..5 satisfying that every
 *            2×2 subblock has diagonal sum differing from antidiagonal sum by
 *            3. Computation performed by a transfer–matrix dynamic program
 *            over row patterns (1296 states)
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- state definitions ------------------------------------------- */
#define STATES     1296          /* 6^4 possible rows */
#define MAX_DEG    64            /* upper bound on outgoing edges per state */

/* state transition tables (built once) */
static unsigned char  outDeg[STATES];
static uint16_t       outEdges[STATES][MAX_DEG];
static int            patDigits[STATES][4];
static int            patDiff[STATES][3];

/* DP buffers */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* cached results for requested n (dynamic array) */
static seq_t *ans = NULL;
static idx_t  ans_cap = 0;     /* allocated capacity */
static idx_t  ans_len = 0;     /* number of terms stored */

/* ------------------------------------------------------------------ */
/* build transition tables and initialize DP buffers                 */
static bool init_done = false;
static void init(void)
{
    if (init_done) return;

    /* enumerate all 4‑tuples of values 0..5 */
    for (int i = 0; i < STATES; ++i) {
        int x = i;
        for (int j = 3; j >= 0; --j) {
            patDigits[i][j] = x % 6;
            x /= 6;
        }
        for (int j = 0; j < 3; ++j)
            patDiff[i][j] = patDigits[i][j + 1] - patDigits[i][j];
    }

    /* build adjacency lists */
    for (int src = 0; src < STATES; ++src) {
        unsigned char deg = 0;
        for (int mask = 0; mask < 8; ++mask) {
            int d_c[3];
            for (int j = 0; j < 3; ++j) {
                int sign = (mask >> j) & 1 ? 1 : -1;
                d_c[j] = patDiff[src][j] + 3 * sign;
            }
            int r[4];
            r[0] = 0;
            r[1] = d_c[0];
            r[2] = d_c[0] + d_c[1];
            r[3] = d_c[0] + d_c[1] + d_c[2];

            int L = -r[0], U = 5 - r[0];
            for (int j = 1; j < 4; ++j) {
                int low  = -r[j];
                int high = 5 - r[j];
                if (low > L)  L = low;
                if (high < U) U = high;
            }
            if (L <= U) {
                for (int c1 = L; c1 <= U; ++c1) {
                    int dst[4];
                    for (int j = 0; j < 4; ++j)
                        dst[j] = c1 + r[j];
                    int idx = ((((dst[0] * 6) + dst[1]) * 6 + dst[2]) * 6 + dst[3]);
                    outEdges[src][deg++] = (uint16_t)idx;
                }
            }
        }
        outDeg[src] = deg;
    }

    /* initialise DP buffer: first row is unrestricted */
    for (int i = 0; i < STATES; ++i)
        dp_cur[i] = (seq_t)1;

    /* allocate answer array (at least two slots) */
    ans_cap = 2;
    ans_len = 2;
    ans = (seq_t *)malloc(sizeof(seq_t) * ans_cap);

    /* compute a(0) (not part of the offset sequence) */
    seq_t z = (seq_t)0;
    for (int i = 0; i < STATES; ++i) z += dp_cur[i];   /* = 1296 */
    ans[0] = z;
    ans_len = 1;
    init_done = true;
}

/* ensure answer array can hold terms up to n */
static void ensure_ans_capacity(idx_t n)
{
    if (n < ans_cap) return;
    idx_t new_cap = ans_cap;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(ans, sizeof(seq_t) * new_cap);
    if (!tmp) abort();
    ans = tmp;
    ans_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* public function required by the build                             */
seq_t a(idx_t n)
{
    if (!init_done) init();
    if (n == 0) return (seq_t)0;          /* not used in the offset */

    /* compute missing terms up to n */
    if (n >= ans_len) {
        ensure_ans_capacity(n);
        for (idx_t step = ans_len; step <= n; ++step) {
            /* zero next DP buffer */
            for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

            /* propagate transitions */
            for (int src = 0; src < STATES; ++src) {
                seq_t val = dp_cur[src];
                if (val == (seq_t)0) continue;
                unsigned char deg = outDeg[src];
                for (unsigned char k = 0; k < deg; ++k) {
                    uint16_t dst = outEdges[src][k];
                    dp_next[dst] += val;
                }
            }

            /* sum all states to obtain a(step) */
            seq_t sum = (seq_t)0;
            for (int i = 0; i < STATES; ++i) sum += dp_next[i];
            ans[step] = sum;

            /* prepare for next iteration (swap buffers) */
            for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
        }
        ans_len = n + 1;
    }
    return ans[n];
}

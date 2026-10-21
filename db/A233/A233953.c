/*
 * A233953  Number of (n+1)X(3+1) 0..5 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 10, and no
 *          two adjacent values equal.
 * https://oeis.org/A233953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×4 arrays of 0..5 with no two adjacent
 *            equal and every 2×2 block sum of squared edge differences = 10.
 *            The algorithm builds all admissible row states, precomputes
 *            their transitions, then uses dynamic programming to count arrays
 *            of each height. The sequence begins at a(1). No...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- constants and types ---------------------------------------- */
#define N_COLS      4                      /* a 3×1 column becomes 4 columns */
#define N_VAL       6                      /* values 0..5 inclusive */
#define MAX_STATES (N_VAL * N_VAL * N_VAL * N_VAL)   /* 6^4 = 1296, but
                                                       horizontal adjacency
                                                       reduces to 750 */

/* A row state: 4 values, with no horizontal repetitions. */
typedef struct { uint8_t v[N_COLS]; } state_t;

/* ---------- global data (initialised lazily) --------------------------- */
static bool          init_done      = false;
static uint16_t      state_count    = 0;
static state_t       states[MAX_STATES];

static uint32_t     *trans_start    = NULL;   /* offset into trans_edges */
static uint16_t     *trans_len      = NULL;   /* number of outgoing edges */
static uint16_t     *trans_edges    = NULL;   /* flattened list of target indices */

static seq_t       *dp_cur         = NULL;   /* counts for current row */
static seq_t       *dp_next        = NULL;   /* scratch for next row */

static seq_t       *results        = NULL;   /* results[1..] */
static size_t       results_cap    = 0;
static idx_t        max_precomp    = 0;

/* ---------- helper functions ------------------------------------------ */
static void init(void)
{
    /* 1. Enumerate all admissible row states. */
    for (uint8_t a = 0; a < N_VAL; ++a)
        for (uint8_t b = 0; b < N_VAL; ++b) if (b != a)
            for (uint8_t c = 0; c < N_VAL; ++c) if (c != b)
                for (uint8_t d = 0; d < N_VAL; ++d) if (d != c) {
                    states[state_count].v[0] = a;
                    states[state_count].v[1] = b;
                    states[state_count].v[2] = c;
                    states[state_count].v[3] = d;
                    state_count++;
                }

    /* 2. Allocate adjacency tables. */
    trans_start = (uint32_t*)malloc(state_count * sizeof(uint32_t));
    trans_len   = (uint16_t*)malloc(state_count * sizeof(uint16_t));

    size_t edges_cap = 200000;     /* will grow if needed */
    trans_edges = (uint16_t*)malloc(edges_cap * sizeof(uint16_t));
    size_t edges_used = 0;

    for (uint16_t i = 0; i < state_count; ++i) {
        const uint8_t *ai = states[i].v;
        trans_start[i] = (uint32_t)edges_used;
        trans_len[i]   = 0;

        for (uint16_t j = 0; j < state_count; ++j) {
            const uint8_t *bj = states[j].v;

            /* Vertical: all four columns must differ. */
            if (ai[0] == bj[0] || ai[1] == bj[1] ||
                ai[2] == bj[2] || ai[3] == bj[3])
                continue;

            /* Check 2×2 block condition. */
            bool ok = true;
            for (uint8_t col = 0; col < N_COLS - 1; ++col) {
                int da = ai[col]   - ai[col+1];
                int db = ai[col]   - bj[col];
                int dc = ai[col+1] - bj[col+1];
                int dd = bj[col]   - bj[col+1];
                int sum = da*da + db*db + dc*dc + dd*dd;
                if (sum != 10) { ok = false; break; }
            }
            if (!ok) continue;

            /* Store transition. */
            if (edges_used == edges_cap) {
                edges_cap *= 2;
                trans_edges = (uint16_t*)realloc(trans_edges,
                                                edges_cap * sizeof(uint16_t));
            }
            trans_edges[edges_used++] = j;
            trans_len[i]++;
        }
    }

    /* 3. DP buffers. */
    dp_cur  = (seq_t*)calloc(state_count, sizeof(seq_t));
    dp_next = (seq_t*)calloc(state_count, sizeof(seq_t));

    /* 4. First row: each state is reachable in one way. */
    for (uint16_t k = 0; k < state_count; ++k) dp_cur[k] = 1;

    /* 5. Results buffer (index 0 unused). */
    results    = (seq_t*)calloc(1, sizeof(seq_t));
    results_cap = 1;

    init_done = true;
}

/* ---------- public function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done) init();
    if (n == 0) return 0;              /* undefined for this sequence */

    /* Extend precomputation if necessary. */
    if (n > max_precomp) {
        size_t needed_cap = (size_t)n + 1;
        if (needed_cap > results_cap) {
            seq_t *tmp = (seq_t*)realloc(results,
                                         needed_cap * sizeof(seq_t));
            results     = tmp;
            results_cap = needed_cap;
        }

        for (idx_t cur_n = max_precomp + 1; cur_n <= n; ++cur_n) {
            /* Zero next DP buffer. */
            memset(dp_next, 0, state_count * sizeof(seq_t));

            /* Transition from current row to next row. */
            for (uint16_t i = 0; i < state_count; ++i) {
                seq_t val = dp_cur[i];
                if (val == 0) continue;
                uint32_t start = trans_start[i];
                uint16_t len   = trans_len[i];
                for (uint16_t k = 0; k < len; ++k) {
                    uint16_t j = trans_edges[start + k];
                    dp_next[j] += val;
                }
            }

            /* Sum dp_next to get total for this n. */
            seq_t total = 0;
            for (uint16_t j = 0; j < state_count; ++j)
                total += dp_next[j];
            results[cur_n] = total;

            /* Prepare for next iteration. */
            seq_t *tmp = dp_cur;
            dp_cur    = dp_next;
            dp_next   = tmp;
        }
        max_precomp = n;
    }
    return results[n];
}

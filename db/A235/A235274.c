/*
 * A235274  Number of (n+1) X (4+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×5 arrays of digits 0..4 such that every
 *            2×2 subblock has its diagonal sum differing from its
 *            antidiagonal sum by exactly 5. Computed by a transfer‑matrix
 *            dynamic programming over all 5⁵ possible row states. This method
 *            runs in O(n·E) time where E ≈ 3×10⁴ is the total number...
 * Range:     u8 none, u16 n <= 9, u32 n <= 22, u64 n <= 46, u128 n <= 96,
 *            big any n
 * Helpers:   sabs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/sabs.h"

/* external integer types supplied by the build system */
extern seq_t a(idx_t n);          /* already declared in the build headers */

#define STATE_COUNT 3125   /* 5^5 */

/* adjacency lists for each row state */
static idx_t *out_list[STATE_COUNT];
static size_t out_len[STATE_COUNT];
static size_t out_cap[STATE_COUNT];

/* pre‑computed digits of each state */
static uint8_t state_digits[STATE_COUNT][5];

/* flag to know whether adjacency has been built */
static int adjacency_built = 0;

/* build adjacency lists once */
static void build_adjacency(void)
{
    /* first encode each state in base‑5 digits */
    for (idx_t s = 0; s < STATE_COUNT; ++s) {
        idx_t tmp = s;
        for (int k = 4; k >= 0; --k) {
            state_digits[s][k] = (uint8_t)(tmp % 5);
            tmp /= 5;
        }
        out_len[s] = 0;
        out_cap[s] = 0;
        out_list[s] = NULL;
    }

    /* enumerate all possible transitions */
    for (idx_t s = 0; s < STATE_COUNT; ++s) {
        uint8_t *a = state_digits[s];

        /* iterate over all 5^5 possible next rows */
        for (int b0 = 0; b0 < 5; ++b0) {
            for (int b1 = 0; b1 < 5; ++b1) {
                if (sabs((int)b1 - (int)b0 + (int)a[0] - (int)a[1]) != 5) continue;
                for (int b2 = 0; b2 < 5; ++b2) {
                    if (sabs((int)b2 - (int)b1 + (int)a[1] - (int)a[2]) != 5) continue;
                    for (int b3 = 0; b3 < 5; ++b3) {
                        if (sabs((int)b3 - (int)b2 + (int)a[2] - (int)a[3]) != 5) continue;
                        for (int b4 = 0; b4 < 5; ++b4) {
                            if (sabs((int)b4 - (int)b3 + (int)a[3] - (int)a[4]) != 5) continue;

                            /* encode the target state */
                            idx_t t = b0
                                    + 5 * (b1 + 5 * (b2 + 5 * (b3 + 5 * b4)));

                            /* grow adjacency array if necessary */
                            if (out_len[s] == out_cap[s]) {
                                size_t newcap = out_cap[s] == 0 ? 8 : out_cap[s] * 2;
                                idx_t *tmp = (idx_t *)realloc(out_list[s],
                                                             newcap * sizeof(idx_t));
                                if (!tmp) abort();  /* allocation failure */
                                out_list[s] = tmp;
                                out_cap[s] = newcap;
                            }
                            out_list[s][out_len[s]++] = t;
                        }
                    }
                }
            }
        }
    }
}

/* dynamic programming arrays */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    if (!adjacency_built) {
        build_adjacency();
        adjacency_built = 1;
    }

    /* initialise for the first row – any state is possible */
    for (idx_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 1;

    /* perform n transitions (array has n+1 rows) */
    for (idx_t step = 1; step <= n; ++step) {
        /* clear next counts */
        for (idx_t i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* propagate counts along edges */
        for (idx_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            for (size_t k = 0; k < out_len[s]; ++k) {
                idx_t t = out_list[s][k];
                dp_next[t] += val;
            }
        }

        /* swap dp_cur and dp_next by copying */
        for (idx_t i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all final states to obtain the total number of arrays */
    seq_t result = 0;
    for (idx_t i = 0; i < STATE_COUNT; ++i) result += dp_cur[i];
    return result;
}

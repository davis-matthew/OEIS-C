/*
 * A120951  Numbers that are not the sum of 5 distinct nonzero squares.
 * https://oeis.org/A120951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 124, u16 n <= 124, u32 n <= 124, u64 n <= 124,
 *            u128 n <= 124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120951: numbers that are *not* the sum of 5 distinct non‑zero squares.
   The maximum such number is 245 (Bateman et al.), so there are exactly 124
   terms.  The function precomputes the list on first call.
   a(1) = 1, a(2) = 2, …, a(124) = 245.  All further indices return 0.
*/
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 245             /* largest n that can occur in the sequence */
#define MAX_SQUARE 15         /* since 15² = 225 ≤ LIMIT < 16² = 256 */

static seq_t non_rep[200];    /* buffer for the 124 non‑representable numbers */
static size_t non_rep_cnt;    /* number of such numbers found */

static bool init_done = false;

static void init_sequence(void)
{
    if (init_done) return;

    static bool representable[LIMIT + 1] = {false};
    idx_t i, j, k, l, m, sum;

    /* enumerate all 5‑element subsets of {1,…,MAX_SQUARE} */
    for (i = 1; i <= MAX_SQUARE - 4; ++i) {
        for (j = i + 1; j <= MAX_SQUARE - 3; ++j) {
            for (k = j + 1; k <= MAX_SQUARE - 2; ++k) {
                for (l = k + 1; l <= MAX_SQUARE - 1; ++l) {
                    for (m = l + 1; m <= MAX_SQUARE; ++m) {
                        sum = i * i + j * j + k * k + l * l + m * m;
                        if (sum <= LIMIT) {
                            representable[sum] = true;
                        }
                    }
                }
            }
        }
    }

    /* collect all numbers ≤ LIMIT that are not representable */
    non_rep_cnt = 0;
    for (idx_t v = 1; v <= LIMIT; ++v) {
        if (!representable[v]) {
            non_rep[non_rep_cnt++] = (seq_t)v;
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* the sequence is 1‑based; indices <1 are undefined */
    init_sequence();         /* precompute once */

    if (n <= (idx_t)non_rep_cnt) {
        return non_rep[n - 1];
    }
    return 0;                /* no term beyond the last one (245) */
}

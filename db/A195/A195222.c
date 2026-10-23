/*
 * A195222  Number of lower triangles of a 4 X 4 integer array with each
 *          element differing from all of its diagonal, vertical, antidiagonal
 *          and horizontal neighbors by n or less and triangles differing by a
 *          constant counted only once.
 * https://oeis.org/A195222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1207573/30240)*n^9 + (1207573/6720)*n^8 +
 *            (1000157/2520)*n^7 + (264247/480)*n^6 + (754417/1440)*n^5 +
 *            (338651/960)*n^4 + (2533393/15120)*n^3 + (90763/1680)*n^2 +
 *            (901/84)*n + 1 (n >= 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 91, u128 n <= 194,
 *            big any n
 * Helpers:   binom_uint128, small_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom_uint128.h"
#include "../utils/small_a.h"

/* If seq_t is an arbitrary‑precision integer class, the
   cast from unsigned __int128 to seq_t must be defined. */
static bool initialized = false;
static seq_t diffs[10];

/* Build the forward difference table Δ^k a(1) for k = 0 .. 9
   from the first ten exact values. */
static void compute_differences(void)
{
    seq_t a[10];
    for (idx_t i = 0; i < 10; ++i) {
        a[i] = small_a(i + 1);          /* a(1) .. a(10) */
    }

    seq_t temp[10];
    for (int i = 0; i < 10; ++i) temp[i] = a[i];
    diffs[0] = temp[0];
    for (int d = 1; d <= 9; ++d) {
        for (int i = 0; i < 10 - d; ++i) {
            temp[i] = temp[i + 1] - temp[i];
        }
        diffs[d] = temp[0];
    }
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_differences();
        initialized = true;
    }

    unsigned __int128 res = (unsigned __int128)diffs[0];   /* Δ^0 = a(1) */

    if (n > 1) {
        unsigned __int128 binom = 1;   /* (n-1 choose 0) */
        for (int k = 1; k < 10; ++k) {
            if (n <= (idx_t)k) break;           /* binomial becomes 0 */
            binom = binom * (unsigned __int128)(n - k) / (unsigned __int128)k;
            res += binom * (unsigned __int128)diffs[k];
        }
    }

    return (seq_t)res;
}

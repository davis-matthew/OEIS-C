/*
 * A239971  Least k>1 such that triangular(n) + triangular(n+k) is a square.
 * https://oeis.org/A239971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 1 such that triangular(n) + triangular(n+k) is
 *            a square
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* triangular(m) = m*(m+1)/2, sum = T(n)+T(n+k) is tested for perfect square. */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute triangular(n) */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    idx_t k = 2;                        /* k must be > 1 */
    while (true) {
        idx_t nk = n + k;               /* n + k */
        /* T(n+k) = (n+k)(n+k+1)/2 */
        seq_t tn_k = (seq_t)nk * ((seq_t)nk + 1) / 2;
        seq_t sum = tn + tn_k;         /* sum to test for square */

        seq_t s = floor_sqrt(sum);     /* floor(sqrt(sum)) */
        if (s * s == sum) {            /* perfect square detected */
            return (seq_t)k;
        }
        k++;                            /* try next candidate */
    }
}

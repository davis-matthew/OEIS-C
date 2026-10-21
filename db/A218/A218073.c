/*
 * A218073  Number of profiles in domino tiling of a 2*n checkboard.
 * https://oeis.org/A218073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   if n even, a(n) = binomial(n, n/2) * n/2; if n odd, a(n) =
 *            binomial(n+1, (n+1)/2) * n/2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 124,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n == 0 gives 0 automatically by the formulas below. */
    if ((n & 1) == 0) {                /* even n */
        idx_t k = n >> 1;              /* n/2 */
        seq_t bin = binomial(n, k);    /* central binomial coefficient */
        seq_t half_n = (seq_t)(n >> 1);/* n/2 as seq_t */
        return bin * half_n;           /* bin * (n/2) */
    } else {                           /* odd n */
        idx_t k = (n + 1) >> 1;        /* (n+1)/2 */
        seq_t bin = binomial(n + 1, k);
        seq_t bin_half = bin >> 1;     /* divide binomial by 2 */
        seq_t nn = (seq_t)n;
        return bin_half * nn;          /* (bin/2) * n */
    }
}

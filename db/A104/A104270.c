/*
 * A104270  a(n) = 2^(n - 2)*(binomial(n,2) + 2).
 * https://oeis.org/A104270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n - 2) * (binomial(n,2) + 2) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* unused, keeps function defined for any idx_t */
    if (n == 1) return 1;           /* special case: 2^(–1) * (0+2) = 1 */

    /* binomial(n,2) = n*(n-1)/2 */
    seq_t binom = (seq_t)n * (seq_t)(n - 1) / 2;
    seq_t base = binom + (seq_t)2;

    /* compute 2^(n-2) by repeated doubling (no shift to avoid UB on 64‑bit) */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 *= (seq_t)2;

    return base * pow2;
}

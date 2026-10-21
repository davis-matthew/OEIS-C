/*
 * A231234  Denominators related to A206771 and Lorentz gamma factor.
 * https://oeis.org/A231234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(2*n-2 - v2(n) - popcount(n-1)) for n>=1; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* v2(n): number of trailing zeros of n */
    idx_t twos = __builtin_ctzll((unsigned long long)n);

    /* popcount(n-1): number of 1 bits in (n-1) */
    idx_t pop = __builtin_popcountll((unsigned long long)(n - 1));

    /* exponent in the power of two */
    idx_t e = 2 * n - 2 - twos - pop;

    /* compute 2^e */
    return pow2(e);
}

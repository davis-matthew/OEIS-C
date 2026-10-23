/*
 * A134828  Numerator of moments of Chebyshev U- (or S-) polynomials.
 * https://oeis.org/A134828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 0 if n odd; otherwise a(n) = numerator(Catalan(n/2)/2^n),
 *            which equals Catalan(n/2)>>popcount(n/2 + 1)-1
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* n is 0-based index. */
    if ((n & 1ULL) != 0)          /* odd n */
        return (seq_t)0;

    /* even n: n = 2*m */
    idx_t m = n >> 1;             /* m = n/2 */

    /* Catalan number C_m */
    seq_t cat = catalan(m);

    /* exponent of 2 in C_m: v2(C_m) = popcount(m+1)-1 */
    unsigned int e = __builtin_popcountll((unsigned long long)(m + 1ULL)) - 1U;

    /* divide by 2^e (shift right by e) to get the numerator */
    return cat >> e;
}

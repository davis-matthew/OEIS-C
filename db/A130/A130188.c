/*
 * A130188  Denominators of rationals r(n) related to the z-sequence of the
 *          Sheffer matrix A060821 for Hermite polynomials.
 * https://oeis.org/A130188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^( n + v2( n+1 ) ) where v2(k) is the exponent of the
 *            highest power of 2 dividing k
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t v2_u64(idx_t x)
{
    idx_t v = 0;
    /* count trailing zeros */
    while ( (x & 1ULL) == 0ULL )
    {
        ++v;
        x >>= 1;
    }
    return v;
}

seq_t a(idx_t n)
{
    /* n+1 is safe for the given range (n up to 10000) */
    idx_t v = v2_u64(n + 1);
    return pow2(n + v);
}

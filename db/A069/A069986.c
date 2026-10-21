/*
 * A069986  Denominator of b(n) = binomial(2n,n)^3*(42n+5)/2^(12n+4).
 * https://oeis.org/A069986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2^(12*n + 4 - 3*popcount(n))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount(n) is the number of 1-bits in the binary representation of n. */
#include "../utils/pow2.h"

static inline idx_t popcount_idx(idx_t x)
{
    idx_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* exponent of 2 in the denominator after reduction */
    idx_t e = (12 * n + 4) - 3 * popcount_idx(n);
    return pow2(e);
}

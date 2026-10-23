/*
 * A035761  Coordination sequence for C_24 lattice.
 * https://oeis.org/A035761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^24
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 15, u128 n <= 108,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t p = 2 * n;               /* 2n */
    seq_t result = 0;
    idx_t lim = (p < 24) ? p : 24; /* i runs from 0 to min(24,2n) */

    for (idx_t i = 0; i <= lim; ++i) {
        seq_t term1 = binom(24, i);          /* binom(24,i)   */
        idx_t n2 = 23 + p - i;               /* 23 + (2n - i) */
        idx_t k  = p - i;                    /* 2n - i        */
        seq_t term2 = binom(n2, k);          /* binom(23+2n-i,2n-i) */
        result += term1 * term2;
    }
    return result;
}

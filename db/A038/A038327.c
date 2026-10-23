/*
 * A038327  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*1^j.
 * https://oeis.org/A038327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 12^(i-j), where i =
 *            floor((sqrt(1+8n)-1)/2) j = n - i*(i+1)/2 and binomial(i,j) =
 *            C(i,j) (triangle entry)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 170, u128 n <= 665,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/* integer square root for unsigned __int128 */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return (idx_t)(lo - 1);
}

seq_t a(idx_t n)
{
    /* Find row index i using triangular numbers */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t i = (isqrt_u128(val) - 1) / 2;

    /* Column index within the row */
    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;

    /* Compute binomial coefficient C(i,j) */
    seq_t bin = binomial(i, j);

    /* Compute 12^(i-j) */
    seq_t pow12 = pow_uint(12, i - j);

    /* Return product */
    return bin * pow12;
}

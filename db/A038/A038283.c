/*
 * A038283  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*5^j.
 * https://oeis.org/A038283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 8^(i-j) * 5^j where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

/* integer square root of a 128‑bit unsigned integer */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 35);   /* sqrt(8*UINT64_MAX+1) < 2^34, so 2^35 is safe */
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* compute i and j from n */
    unsigned __int128 discr = (unsigned __int128)8 * n + 1;
    idx_t i = (isqrt_u128(discr) - 1) / 2;
    unsigned __int128 tri_prev = (unsigned __int128)i * (i + 1) / 2;
    idx_t j = n - (idx_t)tri_prev;

    /* compute the value */
    seq_t bin = binomial(i, j);
    seq_t pow8 = pow_uint(8, i - j);
    seq_t pow5 = pow_uint(5, j);

    return bin * pow8 * pow5;
}

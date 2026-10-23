/*
 * A038294  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*4^j.
 * https://oeis.org/A038294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 9^(i-j) * 4^j where i = floor(
 *            (sqrt(8*n+1)-1)/2 ) j = n - i*(i+1)/2 (i,j) are the antidiagonal
 *            coordinates of index n
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 by binary search. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t i = lo;
    __uint128_t tri_i = (__uint128_t)i * (i + 1) / 2;
    idx_t j = n - (idx_t)tri_i;

    seq_t bin = binomial(i, j);
    seq_t pow9 = big_pow((seq_t)9, (seq_t)(i - j));
    seq_t pow4 = big_pow((seq_t)4, (seq_t)j);
    seq_t result = bin * pow9;
    result = result * pow4;
    return result;
}

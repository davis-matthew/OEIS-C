/*
 * A038258  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*4^j.
 * https://oeis.org/A038258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*6^(i-j)*4^j where (i,j) is the nth entry in
 *            row‑major order of the triangle with offset 0
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Find the row index i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* Determine the row and column of the n-th entry. */
    idx_t i = find_row(n);
    __uint128_t cum = (__uint128_t)i * (i + 1) / 2;
    idx_t j = n - (idx_t)cum;        /* 0 ≤ j ≤ i */

    idx_t exp6 = i - j;
    idx_t exp4 = j;

    /* Compute the value binomial(i,j)*6^(i-j)*4^j. */
    seq_t binom = binomial(i, j);
    seq_t pow6  = big_pow((seq_t)6, (seq_t)exp6);
    seq_t pow4  = big_pow((seq_t)4, (seq_t)exp4);

    seq_t result = binom;
    result *= pow6;
    result *= pow4;
    return result;
}

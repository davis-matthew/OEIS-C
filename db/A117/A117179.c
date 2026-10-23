/*
 * A117179  Riordan array ((1-x^2)/(1+x^2)^2,x/(1+x^2)).
 * https://oeis.org/A117179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = let r be the row index such that r(r+1)/2 <= n <
 *            (r+1)(r+2)/2, k = n - r(r+1)/2. If r-k is odd, a(n)=0. Else s =
 *            (r-k)/2 and a(n) = (-1)^s * ( binomial(k+s+1, s) + binomial(k+s,
 *            s-1) ), where binomial(k+s, -1)=0
 * Range:     i8 n <= 58, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A117179 (Riordan array ((1-x^2)/(1+x^2)^2, x/(1+x^2))), accessed
   by row-major ordering of the triangle.  */

#include <stdbool.h>
#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = 1;
    while (true) {
        __uint128_t s = (__uint128_t)high * (high + 1) / 2;
        if (s > n) break;
        /* Prevent overflow of high */
        if (high > (n >> 1)) { high = n + 1; continue; }
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t s = (__uint128_t)mid * (mid + 1) / 2;
        if (s <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    __uint128_t start128 = (__uint128_t)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;          /* fits in idx_t by construction */
    idx_t k = n - start;

    /* If (r-k) is odd, the entry is zero. */
    if (((r - k) & 1U) != 0U)
        return (seq_t)0;

    idx_t s = (r - k) >> 1;                 /* (r-k)/2 */

    seq_t c1 = binomial(k + s + 1, s);
    seq_t c2 = (s > 0) ? binomial(k + s, s - 1) : (seq_t)0;
    seq_t sum = c1 + c2;

    if ((s & 1U) != 0U)
        sum = -sum;

    return sum;
}

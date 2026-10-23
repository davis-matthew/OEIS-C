/*
 * A124751  Expansion of (1+x^2+x^4)/(1-x^6+x^7).
 * https://oeis.org/A124751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n * sum_{k=0..floor(n/2)} binom(floor(k/3), n-2k)
 * Range:     i8 n <= 50, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"


seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k) {
        idx_t m = k / 3;
        idx_t r = n - 2 * k;
        if (r > m) continue;
        sum += binom(m, r);
    }
    return (n & 1) ? -sum : sum;
}

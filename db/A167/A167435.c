/*
 * A167435  Hankel transform of A084076.
 * https://oeis.org/A167435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2^(n^2)*(-1)^n*Sum_{k=0..n} C(n+k,2k)*(-2)^(n-k)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Accumulate the binomial sum */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin   = binom(n + k, 2 * k);                     /* C(n+k,2k)   */
        seq_t pow   = big_pow((seq_t)-2, (seq_t)(n - k));      /* (-2)^(n-k)  */
        sum += bin * pow;
    }

    /* Apply (-1)^n */
    if (n & 1u)
        sum = -sum;

    /* Multiply by 2^(n^2) */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n * n));

    return sum * pow2;
}

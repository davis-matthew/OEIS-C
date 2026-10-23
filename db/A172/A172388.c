/*
 * A172388  a(n) = Sum_{k=0..n} (-1)^k*C(n,k)*2^(k*(n-k)).
 * https://oeis.org/A172388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * C(n,k) * 2^(k*(n-k))
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 15, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* odd-indexed terms are always zero */
    if (n & 1ULL) return 0;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term = binom(n, k);                       /* C(n,k) */
        seq_t exp  = (seq_t)k * (seq_t)(n - k);          /* k*(n-k)   */
        term = term * big_pow((seq_t)2, exp);            /* *2^(k*(n-k)) */
        if (k & 1ULL) term = -term;                     /* (-1)^k    */
        sum += term;
    }
    return sum;
}

/*
 * A235328  Number of ordered pairs of endofunctions (f,g) on a set of n
 *          elements satisfying f(x) = g(f(f(x))).
 * https://oeis.org/A235328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k! * binom(n,k) * (n*k)^(n-k) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Computes the nth term of OEIS sequence A235328. */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum   = 0;   /* running total of the sum */
    seq_t fact  = 1;   /* k!  (cumulative) */
    seq_t binom = 1;   /* C(n, k) (cumulative) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial: fact = fact * k */
        fact *= (seq_t)k;

        /* Update binomial coefficient: binom = binom * (n - k + 1) / k */
        binom = binom * (seq_t)(n - k + 1) / (seq_t)k;

        /* Base for the power: n * k */
        seq_t base = (seq_t)n * (seq_t)k;

        /* Exponent: n - k */
        seq_t exp = (seq_t)(n - k);

        /* Compute power: base^exp */
        seq_t power = big_pow(base, exp);

        /* Term: k! * C(n, k) * (n*k)^(n-k) */
        seq_t term = fact * binom * power;

        /* Add to total */
        sum += term;
    }

    return sum;
}

/*
 * A129840  a(n) = n! * Sum_{k=1..n} binomial(2n+1,k)/k.
 * https://oeis.org/A129840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} binomial(2n+1,k)/k Computes the OEIS
 *            sequence A129840
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n == 0 return 0. */
    if (n == 0)
        return 0;

    seq_t fact = factorial(n);  /* n! */
    seq_t sum  = 0;
    seq_t bin  = 1;             /* binomial(2n+1,0) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t num = 2 * n + 2 - k;          /* part of the multiplicative formula */
        bin = bin * (seq_t)num;             /* multiply by upper factor */
        bin = bin / (seq_t)k;               /* divide by k → binomial(2n+1,k) */

        seq_t term = fact * bin;            /* n! * binomial(2n+1,k) */
        term = term / (seq_t)k;             /* divide by k as per the definition */

        sum += term;
    }

    return sum;
}

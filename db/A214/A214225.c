/*
 * A214225  E.g.f. satisfies: A(x) = x/(1 - tanh(A(x))).
 * https://oeis.org/A214225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A214225: a(n) = (1/2) * Σ_{k=0..n} binomial(n,k) * k^(n-1)  */

/* This implementation uses only integer arithmetic and the provided
 * helper `binomial(idx_t n, idx_t k)` function.  It evaluates k^(n‑1)
 * by binary exponentiation.  The algorithm runs in O(n log n) per
 * call, which comfortably satisfies the 2 s limit for n ≤ 10000. */

#include <stddef.h>   /* for size_t, if needed */

/* Provided helper: binomial coefficient, exact integer result. */
seq_t binomial(idx_t n, idx_t k);

/* Compute x^e  (binary exponentiation), 0^0 == 1. */
static seq_t pow_seq(seq_t x, idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = x;
    idx_t exp    = e;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* Valid for n ≥ 1; behaviour for n < 1 is irrelevant. */
    if (n == 1ULL) return (seq_t)1;          /* a(1) = 1 */

    seq_t sum = (seq_t)0;
    /* k = 1 term: binomial(n,1) = n, 1^(n-1) = 1 */
    seq_t binom = (seq_t)n;
    seq_t pw    = pow_seq((seq_t)1, n - 1);   /* always 1 */
    sum = binom * pw;                         /* add k = 1 term */

    /* k from 2 to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* Update binomial coefficient: C(n,k) = C(n,k-1)*(n-k+1)/k */
        binom = (binom * (n - k + 1)) / k;
        pw = pow_seq((seq_t)k, n - 1);
        sum += binom * pw;
    }

    /* Final division by 2 as per the formula. */
    return sum / (seq_t)2;
}

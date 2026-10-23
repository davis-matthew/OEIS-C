/*
 * A196839  Triangle of denominators of the coefficient of x^m in the n-th
 *          Bernoulli polynomial, 0 <= m <= n.
 * https://oeis.org/A196839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom_divisible_by_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A196839: Triangle of denominators of the coefficient of
   x^m in the n‑th Bernoulli polynomial.
   Implementation uses the formula
        T(n,m) = denominator( binomial(n,m) * B_{n-m} )
   where the denominator of the Bernoulli number B_r (r ≥ 0) is
   the product of all primes p such that p‑1 divides r
   (for odd r > 1 the Bernoulli number is zero, so the denominator is 1).
   The linear index supplied to a(idx_t n) follows the OEIS ordering
   (row by row, left to right).  The function returns the exact
   denominator as a seq_t value. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom_divisible_by_prime.h"

/* ------------------------------------------------------------ */
/* Small list of primes up to 199 (needed for all possible r ≤ 140).
   The list is static and fully known at compile time.  */
static const int primes[46] = {
     2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97,101,103,107,109,113,
   127,131,137,139,149,151,157,163,167,173,
   179,181,191,193,197,199
};
static const int prime_count = 46;

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Return the denominator of the Bernoulli number B_r.
   For r=0 return 1; for r=1 return 2; for odd r>1 return 1. */
static seq_t bernoulli_denom(idx_t r)
{
    if (r == 0)          return (seq_t)1;
    if (r == 1)          return (seq_t)2;
    if ((r & 1))         return (seq_t)1;   /* odd r > 1 */

    seq_t denom = (seq_t)1;
    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if ((idx_t)(p - 1) > r) break;
        if (r % (idx_t)(p - 1) == 0)
            denom *= (seq_t)p;
    }
    return denom;
}

/* ------------------------------------------------------------ */
/* Main sequence function.  Maps linear index n to row k and column m
   in the triangular table, then returns the denominator
   T(k,m) = denominator( binomial(k,m) * B_{k-m} ).               */
seq_t a(idx_t n)
{
    /* Find the row k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2. */
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n) ++k;
    idx_t m = n - k * (k + 1) / 2;

    idx_t r = k - m;           /* r = n - m */
    if (r > 1 && (r & 1))      /* odd r > 1 => Bernoulli number zero   */
        return (seq_t)1;

    seq_t denom = bernoulli_denom(r);

    /* Cancel common prime factors that divide the binomial coefficient
       (binom(k,m)).  Since denom is square‑free, simply remove each
       prime that also divides the binomial. */
    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if ((idx_t)(p - 1) > r) break;
        if (r % (idx_t)(p - 1) == 0) {
            if (binom_divisible_by_prime(k, m, p))
                denom /= (seq_t)p;
        }
    }

    return denom;
}

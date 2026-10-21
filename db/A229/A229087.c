/*
 * A229087  a(n) = sigma(n) mod n - antisigma(n) mod n, where sigma(n) =
 *          A000203(n) = sum of divisor of n, antisigma(n) = A024816(n) = sum
 *          of non-divisors of n.
 * https://oeis.org/A229087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma(n) mod n - antisigma(n) mod n, where sigma(n) = sum
 *            of divisors of n, antisigma(n) = sum of non‑divisors of n
 * Range:     i8 n <= 15, i16 n <= 1041, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t if needed */
#include <stdlib.h>   /* for abort() */

/* Compute sigma(n) by iterating divisors up to sqrt(n). */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    for (i = 1; (idx_t)i * i <= n; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t d2 = n / i;
            if (d2 != i) sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety: sequence defined for n>=1 */

    seq_t sigma = sigma_of(n);           /* sum of divisors */
    seq_t nseq  = (seq_t)n;
    seq_t sigma_mod = sigma % nseq;

    /* Compute T = n(n+1)/2 modulo n without overflow. */
    seq_t T = ((seq_t)n * (seq_t)(n + 1)) / 2;
    seq_t T_mod = T % nseq;

    /* r2 = (T_mod - sigma_mod) mod n */
    seq_t r2 = (T_mod - sigma_mod + nseq) % nseq;

    /* a(n) = sigma_mod - r2  (may be negative) */
    return sigma_mod - r2;
}

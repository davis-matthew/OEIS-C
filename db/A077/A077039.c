/*
 * A077039  Partial sums of A073579.
 * https://oeis.org/A077039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = -∑_{k=2..n} A000040(k)*i^(A000040(k)+1)
 * Range:     i8 n <= 21, i16 n <= 642, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent real implementation: sum of primes p_k (k≥2) with sign
   +p if p≡1 (mod 4), –p if p≡3 (mod 4); prime 2 contributes 0. */

#include <stddef.h>   /* for size_t, NULL */
#include <stdlib.h>   /* for malloc, free, calloc */
#include <math.h>     /* for log */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 0 by definition.   */
    if (n <= 1)
        return (seq_t)0;

    /* Estimate upper bound for the n‑th prime.  */
    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double dn = (double)n;
        double upper = dn * (log(dn) + log(log(dn))) + 3.0;
        limit = (idx_t)upper;
    }

    /* Allocate sieve array: 1 = prime candidate, 0 = composite.  */
    unsigned char *is_prime = (unsigned char *)calloc((size_t)limit + 1,
                                                       sizeof(*is_prime));
    if (!is_prime)          /* allocation failure – give best effort */
        return (seq_t)0;

    /* Except for 0 and 1, initially mark all as potentially prime.  */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    /* Simple Eratosthenes sieve.  */
    idx_t sq = (idx_t)sqrt((double)limit);
    for (idx_t p = 2; p <= sq; ++p) {
        if (!is_prime[p])
            continue;
        for (idx_t m = p * p; m <= limit; m += p) {
            is_prime[m] = 0;
        }
    }

    /* Accumulate the partial sum.  Use a wider intermediate type to
       preserve exactness for small seq_t builds.  */
    long long sum_ll = 0;
    idx_t prime_index = 0;            /* count of primes seen so far   */
    for (idx_t p = 2; p <= limit && prime_index < n; ++p) {
        if (!is_prime[p])
            continue;

        prime_index++;              /* prime at position prime_index */

        if (prime_index == 1)       /* the first prime is 2 – skip */
            continue;

        if ((p & 3ULL) == 1ULL)     /* p ≡ 1 (mod 4)  → +p */
            sum_ll += (long long)p;
        else                        /* p ≡ 3 (mod 4)  → –p */
            sum_ll -= (long long)p;
    }

    free(is_prime);
    return (seq_t)sum_ll;
}

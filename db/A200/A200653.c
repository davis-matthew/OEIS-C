/*
 * A200653  a(n) = greatest k with -3<k<2*n such that n*(n+1)+k and
 *          n*(n+1)+k+2 are twin primes.
 * https://oeis.org/A200653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = greatest k with -3 < k < 2*n such that * n*(n+1)+k and
 *            n*(n+1)+k+2 are twin primes
 * Range:     i8 n <= 10, i16 n <= 409, i32 n <= 409, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation tests each candidate k in descending order
 * and uses the utility function is_prime_big() to check primality. */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* main base = n*(n+1) */
    seq_t base = (seq_t)n * ((seq_t)n + 1);

    /* Search from k = 2*n-1 down to k = -2 (since k > -3). */
    for (int64_t k = (int64_t)2 * (int64_t)n - 1; k >= -2; --k)
    {
        seq_t N = base + (seq_t)k;
        /* Twin primes must be at least 2; for n>=1 this is always true. */
        if (is_prime_big(N) && is_prime_big(N + 2))
            return (seq_t)k;
    }
    /* If no suitable k is found, return 0. */
    return (seq_t)0;
}

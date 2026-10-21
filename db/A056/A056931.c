/*
 * A056931  Difference between n-th oblong (promic) number, n(n+1), and the
 *          average of the smallest prime greater than n^2 and the largest
 *          prime less than (n+1)^2.
 * https://oeis.org/A056931
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*(n+1) - ((nextPrime(n^2) + prevPrime((n+1)^2)) / 2)
 * Range:     i8 n <= 7, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts use the smallest prime > n^2 and the largest prime < (n+1)^2. */

#include <stdint.h>
#include <stdbool.h>

/* Simple primality test sufficient for n <= ~1.1e8 */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

/* Smallest prime strictly greater than start */
static uint64_t next_prime(uint64_t start)
{
    while (!is_prime_uint64(start))
        ++start;
    return start;
}

/* Largest prime strictly less than start */
static uint64_t prev_prime(uint64_t start)
{
    while (!is_prime_uint64(start))
        --start;
    return start;
}

seq_t a(idx_t n)
{
    /* Only values for n >= 2 are meaningful; below offset can be arbitrary. */
    if (n < 2)
        return 0;

    uint64_t n64 = (uint64_t) n;
    uint64_t n2   = n64 * n64;
    uint64_t n1sq = (n64 + 1) * (n64 + 1);

    /* Find required primes. */
    uint64_t p_small = next_prime(n2 + 1);   /* smallest prime > n^2 */
    uint64_t p_large = prev_prime(n1sq - 1); /* largest prime < (n+1)^2 */

    /* Compute the average of the two primes. */
    seq_t prime_sum = (seq_t)(p_small + p_large);   /* sum fits in seq_t for all n of interest */
    seq_t avg_prime = prime_sum / 2;                /* integer division – the sum is always even */

    /* Compute n*(n+1) in seq_t to avoid overflow. */
    seq_t oblong = (seq_t)n * ((seq_t)n + 1);

    return oblong - avg_prime;
}

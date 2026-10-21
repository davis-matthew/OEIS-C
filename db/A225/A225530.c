/*
 * A225530  Number of ordered pairs (i,j) with i,j >= 0, i + j = n and
 *          gcd(i,j) <= 1.
 * https://oeis.org/A225530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = φ(n) for n ≥ 2; a(0) = 1, a(1) = 2
 * Range:     u8 n <= 255, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for primes used by the totient calculation. */
static uint64_t *primes = NULL;      /* prime numbers */
static size_t   primes_cnt = 0;      /* number of primes stored */
static idx_t    primes_limit = 0;    /* largest prime generated */

/* Build a list of primes up to 'limit' using a simple sieve. */
static void build_primes_upto(idx_t limit)
{
    if (limit < 2) {
        primes_limit = limit;
        return;
    }
    size_t sz = (size_t)(limit + 1);
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    uint64_t *new_pr = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_pr) { free(is_prime); exit(1); }
    primes = new_pr;
    primes_cnt = cnt;

    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[pos++] = (uint64_t)i;

    free(is_prime);
    primes_limit = limit;
}

/* Ensure that primes up to 'limit' are available. */
static void ensure_primes(idx_t limit)
{
    if (primes_limit >= limit) return;
    build_primes_upto(limit);
}

/* Compute Euler's totient φ(n) for n ≥ 1, returning a seq_t. */
static seq_t totient(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* should not be called for n == 0 */
    seq_t result = (seq_t)n;
    idx_t temp = n;
    idx_t sqr = (idx_t)((double)n * 0.5 + 0.5); /* upper bound for sqrt approx */
    if (sqr * sqr < n) ++sqr;   /* ensure sqr^2 >= n */
    ensure_primes(sqr);

    for (size_t idx = 0; idx < primes_cnt; ++idx) {
        uint64_t p = primes[idx];
        if ((idx_t)p * (idx_t)p > temp) break;
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) {
        result -= result / (seq_t)temp;
    }
    return result;
}

/* Main function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    return totient(n);
}

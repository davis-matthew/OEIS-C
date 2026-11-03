/*
 * A186971  Maximal cardinality of a subset of {1, 2, ..., n} containing n and
 *          having pairwise coprime elements.
 * https://oeis.org/A186971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n (n < 4) a(n) = pi(n) - omega(n) + 2 (n >= 4) pi(n) :
 *            number of primes <= n (A000720) omega(n): number of distinct
 *            prime divisors of n (A001221)
 * Range:     u8 n <= 1612, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9792 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* prime counting function pi(x) using a simple sieve */
static seq_t prime_pi(idx_t limit) {
    if (limit < 2) return 0;
    uint8_t *is_prime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!is_prime) return 0;          /* out of memory → 0 as a safe fallback */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t q = p * p; q <= limit; q += p) is_prime[q] = 0;
        }
    }
    seq_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++count;
    free(is_prime);
    return count;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)n;

    seq_t pi = prime_pi(n);
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    return pi - omega + 2;
}

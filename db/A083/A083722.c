/*
 * A083722  Product of primes greater than the greatest prime factor of n but
 *          not greater than n.
 * https://oeis.org/A083722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 31, u64 n <= 59, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = product of all primes p with  max_prime_factor(n) < p ≤ n.
 *   a(n) = 1 if n is prime or n = 1.
 *   Example: a(8) = 3·5·7 = 105.
 *
 * The function keeps a cached list of primes up to the largest n ever
 * requested.  The cache is rebuilt only when a larger n is supplied.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Cached list of primes. */
static idx_t *prime_list  = NULL;  /* indices 0 … prime_cnt-1 are primes. */
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;     /* largest n for which primes are up to date. */

/* Rebuild the cached prime list up to at least 'limit'.
 * Uses a classic sieve of Eratosthenes.  Memory consumption is O(limit). */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                 /* already up to date. */

    /* Sieve of Eratosthenes up to 'limit'. */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        exit(1);                /* out of memory – abort. */

    /* Initially mark everything as prime. */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    if (sz > 0) is_prime[0] = false;
    if (sz > 1) is_prime[1] = false;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes. */
    size_t count = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i]) ++count;

    /* Allocate and fill prime list. */
    idx_t *plist = (idx_t *)malloc(count * sizeof(idx_t));
    if (!plist)
        exit(1);                 /* out of memory – abort. */
    size_t pos = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            plist[pos++] = (idx_t)i;

    free(is_prime);

    /* Replace old list. */
    free(prime_list);
    prime_list  = plist;
    prime_cnt   = count;
    prime_limit = limit;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Ensure we have all primes up to n, since the product requires them. */
    ensure_primes(n);

    /* Find the greatest prime factor of n. */
    idx_t temp = n;
    idx_t lpf  = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > temp / p)               /* p > sqrt(temp) */
            break;
        if (temp % p == 0) {
            lpf = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1 && temp > lpf)          /* remaining prime factor > sqrt(original) */
        lpf = temp;

    /* Compute product of primes > lpf and ≤ n. */
    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n)
            break;
        if (p > lpf)
            prod *= (seq_t)p;
    }

    return prod;
}

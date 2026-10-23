/*
 * A186994  Number of maximum subsets of {1, 2, ..., n} containing n and
 *          having pairwise coprime elements.
 * https://oeis.org/A186994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, hear
 * Formula:   a(n) = ∏_{p<n, gcd(n,p)=1} ⌊log_p(n)⌋ (product over all primes p
 *            less than n that do not divide n)
 * Range:     u8 n <= 126, u16 n <= 966, u32 n <= 6898, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* cached list of primes */
static idx_t *prime_arr = NULL;      /* array of primes */
static size_t prime_cnt = 0;         /* number of primes stored */
static idx_t prime_limit = 0;        /* highest n for which primes are valid */

/* build or extend the prime list up to (but not including) limit+1 */
static void ensure_primes(idx_t limit_plus_one)
{
    if (prime_limit >= limit_plus_one) return;   /* already sufficient */

    /* allocate scratch array for sieve */
    bool *is_prime = (bool *)malloc((size_t)(limit_plus_one) * sizeof(bool));
    if (!is_prime) {                            /* allocation failed, give up */
        return;
    }

    /* initialize sieve */
    for (idx_t i = 0; i < limit_plus_one; ++i)
        is_prime[i] = true;
    if (limit_plus_one > 0) is_prime[0] = false;
    if (limit_plus_one > 1) is_prime[1] = false;

    /* classic Sieve of Eratosthenes */
    for (idx_t i = 2; i * i < limit_plus_one; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j < limit_plus_one; j += i)
                is_prime[j] = false;

    /* collect primes into prime_arr */
    size_t new_cnt = 0;
    for (idx_t i = 2; i < limit_plus_one; ++i)
        if (is_prime[i]) ++new_cnt;

    /* reallocate storage for primes */
    size_t old_size = prime_limit + 1;
    size_t new_size = limit_plus_one;
    idx_t *new_arr = (idx_t *)realloc(prime_arr, new_size * sizeof(idx_t));
    if (!new_arr) {       /* allocation failed, keep old data */
        free(is_prime);
        return;
    }
    prime_arr = new_arr;

    /* copy primes */
    size_t idx = 0;
    for (idx_t i = 2; i < limit_plus_one; ++i)
        if (is_prime[i])
            prime_arr[idx++] = i;
    prime_cnt = new_cnt;
    prime_limit = limit_plus_one - 1;

    free(is_prime);
}

/* floor(log_p(n)) for prime p < n */
static int floor_log_p(idx_t n, idx_t p)
{
    int k = 0;
    idx_t power = p;
    while (power <= n) {
        ++k;
        if (power > n / p) break; /* next multiplication would exceed n */
        power *= p;
    }
    return k;                     /* at least 1 because p < n */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside the official offset */

    /* ensure we have all primes < n */
    ensure_primes(n);

    seq_t prod = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (n % p == 0) continue;   /* skip primes dividing n */
        int k = floor_log_p(n, p);
        prod *= (seq_t)k;
    }
    return prod;
}

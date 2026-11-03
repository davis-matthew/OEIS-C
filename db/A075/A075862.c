/*
 * A075862  Numbers m such that the least k such that (m-k) divides (m+k) is
 *          prime.
 * https://oeis.org/A075862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A075862 – the sequence of integers m such that
 *  the least k with (m − k) | (m + k) is a prime.
 *  For each m we find dmax = max{d | d | 2m , d < m}
 *  and test whether k = m − dmax is prime.
 *  The implementation scans integers ≥ 4, stores all
 *  qualifying terms, and memoises the results so that
 *  arbitrary indices can be answered efficiently. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

/* Return the largest divisor of (2*m) that is < m.
 * The search is performed by scanning all divisors up to sqrt(2*m). */
static seq_t largest_divisor_lt(uint64_t m, uint64_t two_m)
{
    seq_t best = 1;
    for (uint64_t d = 1; d * d <= two_m; ++d) {
        if (two_m % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = two_m / d;
            if (d1 < m && d1 > (uint64_t)best) best = (seq_t)d1;
            if (d2 < m && d2 > (uint64_t)best) best = (seq_t)d2;
        }
    }
    return best;
}

#define PRIME_LIMIT 2000000   /* > 2 * maximal m expected for n <= 10000 */

static bool prime_is_prime[PRIME_LIMIT + 1] = {0};
static bool prime_inited = false;

/* Simple sieve of Eratosthenes, executed once on demand */
static void init_primes(void)
{
    if (prime_inited) return;
    prime_inited = true;
    for (int i = 0; i <= PRIME_LIMIT; ++i) prime_is_prime[i] = true;
    prime_is_prime[0] = prime_is_prime[1] = false;
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (prime_is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                prime_is_prime[j] = false;
}

/* Cache of computed sequence values.  Cache[0] holds a(1). */
static seq_t *term_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 4;   /* next integer to be tested for membership */

/* Ensure the cache has room for at least 'need' terms. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = mem_resize(seq_t, term_cache, new_cap);
    term_cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – value not defined for n < 1 */

    init_primes();
    if (cache_len < (size_t)n) {
        ensure_cache_capacity((size_t)n);
        while (cache_len < (size_t)n) {
            uint64_t m = next_candidate++;
            uint64_t t = 2 * m;
            seq_t dmax = largest_divisor_lt(m, t);
            uint64_t k = m - (uint64_t)dmax;
            if (k >= 2 && k <= PRIME_LIMIT && prime_is_prime[(int)k]) {
                term_cache[cache_len++] = (seq_t)m;
            }
        }
    }
    return term_cache[n - 1];
}

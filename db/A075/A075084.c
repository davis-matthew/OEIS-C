/*
 * A075084  Number of composite numbers c with n <= c <= 2*n.
 * https://oeis.org/A075084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - pi(2*n) + pi(n-1) + 1 (n > 1)
 * Range:     u8 n <= 302, u16 n <= 9855, u32 n <= 9855, u64 n <= 9855,
 *            u128 n <= 9855, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache of prime‑count prefix */
static seq_t *pi_cache = NULL;
static idx_t  pi_cache_limit = 0;

/* Build a prime indicator array and compute prefix prime counts up to limit */
static void build_pi_cache(idx_t limit)
{
    idx_t i, p, m, lim = limit;
    uint8_t *is_prime = (uint8_t *)malloc((lim + 1) * sizeof(uint8_t));
    if (!is_prime) exit(1);

    for (i = 0; i <= lim; ++i) is_prime[i] = 1;
    if (lim >= 0) is_prime[0] = 0;
    if (lim >= 1) is_prime[1] = 0;

    for (p = 2; (idx_t)p * p <= lim; ++p) {
        if (is_prime[p]) {
            for (m = (idx_t)p * p; m <= lim; m += p) is_prime[m] = 0;
        }
    }

    seq_t *new_cache = (seq_t *)malloc((lim + 1) * sizeof(seq_t));
    if (!new_cache) { free(is_prime); exit(1); }

    seq_t count = 0;
    for (i = 0; i <= lim; ++i) {
        if (is_prime[i]) ++count;
        new_cache[i] = count;
    }

    if (pi_cache) free(pi_cache);
    pi_cache        = new_cache;
    pi_cache_limit  = lim;

    free(is_prime);
}

/* Ensure that the cache covers at least up to needed */
static void ensure_pi_cache(idx_t needed)
{
    if (!pi_cache || needed > pi_cache_limit) build_pi_cache(needed);
}

/*  OEIS A075084: number of composite numbers c with n ≤ c ≤ 2n */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0; a(0) is unused */

    idx_t two_n = n + n;
    ensure_pi_cache(two_n);

    seq_t pi_2n = pi_cache[two_n];
    seq_t pi_n1 = pi_cache[n - 1];

    /* a(n) = n - pi(2n) + pi(n-1) + 1   for n > 1 */
    return (seq_t)n - pi_2n + pi_n1 + 1;
}

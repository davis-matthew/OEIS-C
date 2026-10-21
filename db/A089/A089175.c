/*
 * A089175  a(n) = floor(sqrt(prime(n)*primepi(n))).
 * https://oeis.org/A089175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(sqrt(prime(n) * primepi(n)))
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Cache of primes computed by a simple sieve */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t sieve_limit = 0;

/* Rebuild the prime sieve up to 'limit' (inclusive) */
static void rebuild_sieve(idx_t limit) {
    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return;               /* OOM fallback */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    uint32_t *pnew = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) pnew[idx++] = (uint32_t)i;
    free(is_prime);
    free(primes);
    primes = pnew;
    prime_cnt = cnt;
    sieve_limit = limit;
}

/* Ensure that we have at least 'n' primes cached and that the sieve
   covers primes up to at least 'n' itself. */
static void ensure_primes(idx_t n) {
    if (prime_cnt >= n) return;
    idx_t limit = (n < 6) ? 20 : n * 20;   /* generous upper bound */
    if (limit <= sieve_limit) {
        /* Still lacking enough primes, double the limit. */
        while (prime_cnt < (size_t)n) {
            limit *= 2;
        }
        rebuild_sieve(limit);
    } else {
        rebuild_sieve(limit);
        if (prime_cnt < (size_t)n) {
            limit *= 2;
            rebuild_sieve(limit);
        }
    }
}

/* Count of primes <= x (π(x)). */
static idx_t prime_pi(idx_t x) {
    if (prime_cnt == 0) return 0;
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)primes[mid] <= x) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)lo;
}

/* OEIS sequence A089175 */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_primes(n);
    seq_t prime_n = primes[n - 1];
    seq_t pi_n = prime_pi(n);
    seq_t prod = prime_n * pi_n;
    return floor_sqrt(prod);
}

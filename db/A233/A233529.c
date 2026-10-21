/*
 * A233529  a(n) = |{0 < k <= n/2: prime(k)*prime(n-k) - 6 is prime}|.
 * https://oeis.org/A233529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ 0 < k <= n/2 : prime(k)*prime(n-k) - 6 is prime }|
 * Range:     u8 n <= 3107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/prime_uint64.h"

/* Global storage for prime numbers up to the current maximum n ever
   requested. */
static uint64_t *prime_list   = NULL;   /* array of primes (1‑based indices)   */
static size_t   prime_count   = 0;      /* number of primes stored            */

/* Generates all primes up to 'limit' (inclusive) and stores them in
   prime_list.  Old data are freed. */
static void generate_primes_upto(size_t limit) {
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = (uint64_t)i;

    free(prime_list);
    free(is_prime);
    prime_list = pr;
    prime_count = cnt;
}

/* Ensures that at least 'n' primes are available in prime_list. */
static void ensure_primes(idx_t n) {
    if (prime_count >= (size_t)n) return;

    double dn = (double)n;
    size_t limit;
    if (n < 6) limit = 15;
    else limit = (size_t)(dn * (log(dn) + log(log(dn))) + 10);

    /* Increase limit until we have enough primes. */
    while (true) {
        generate_primes_upto(limit);
        if (prime_count >= (size_t)n) break;
        limit = limit * 2 + 10;
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_primes(n);

    idx_t half = n >> 1;               /* n/2 */
    size_t count = 0;

    for (idx_t k = 1; k <= half; ++k) {
        uint64_t p = prime_list[(size_t)k - 1];          /* prime(k)   */
        uint64_t q = prime_list[(size_t)n - (size_t)k - 1]; /* prime(n-k) */
        uint64_t product = p * q;
        if (product <= 6) continue; /* product-6 would not be positive */
        uint64_t val = product - 6;
        if (prime_uint64(val)) ++count;
    }
    return (seq_t)count;
}

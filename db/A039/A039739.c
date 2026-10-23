/*
 * A039739  a(n) = 2*q-prime(n), where q is the prime < prime(n) for which
 *          (prime(n) mod q) is maximal.
 * https://oeis.org/A039739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*q - prime(n) where q is the prime < prime(n) for which
 *            (prime(n) mod q) is maximal
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to generate all primes up to LIMIT using a simple sieve. */
static void build_primes_upto(uint64_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    /* Allocate a bit array for the sieve */
    uint8_t *sieve = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve) return;
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 0;  /* 0 means prime candidate */

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!sieve[p]) {
            for (uint64_t m = p * p; m <= limit; m += p)
                sieve[m] = 1;   /* mark composite */
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) ++cnt;

    /* Allocate primes array */
    uint32_t *primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) {
        free(sieve);
        return;
    }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
    *primes_out = primes;
    *cnt_out = cnt;
}

/* The sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* n below offset not used */

    /* Static buffers for primes and cached results. */
    static bool init = false;
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t max_computed = 1;    /* nothing computed yet */

    if (!init) {
        /* Decide a safe upper bound for the sieve: > 10,000th prime (104729). */
        uint64_t LIMIT = 200000;
        build_primes_upto(LIMIT, &primes, &primes_cnt);
        cache_cap = primes_cnt + 2;    /* room for n up to primes_cnt+1 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        init = true;
    }

    if ((size_t)n > primes_cnt) {
        /* Outside precomputed range – return 0 as undefined. */
        return 0;
    }

    /* Compute all values up to n (memoized). */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        /* i corresponds to a(i).  primes[i-1] is prime(i). */
        uint32_t p = primes[i - 1];
        uint32_t best_q = 2;          /* at least 2 */
        uint32_t best_r = 0;

        for (size_t j = 0; j < i - 1; ++j) {
            uint32_t q = primes[j];
            uint32_t r = p % q;
            if (r > best_r) {
                best_r = r;
                best_q = q;
            }
        }
        cache[i] = (seq_t)2 * (seq_t)best_q - (seq_t)p;
    }
    max_computed = (size_t)n;
    return cache[n];
}

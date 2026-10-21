/*
 * A088489  For each pair of twin primes (p,p+2) take the absolute value of
 *          the difference between p and p with digits reversed.
 * https://oeis.org/A088489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = |p - rev(p)| where (p,p+2) is the n-th twin prime pair.
 *            The sequence is enumerated in increasing order of the lower
 *            prime p
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   grow_primes, grow_seq_cache, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/grow_primes.h"
#include "../utils/grow_seq_cache.h"

/* Simple trial‑division primality test using the list of primes discovered so far. */
static bool is_prime_u64(uint64_t n, uint64_t *primes, size_t primes_len) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > n / p) break;          /* avoid overflow    */
        if (n % p == 0) return false;
    }
    return true;
}

/* Computes the n-th term of A088489 (1‑based index). */
seq_t a(idx_t n) {
    /* static state ----------------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;

    static uint64_t current = 3;          /* next odd candidate to test */

    /* initialise on first call ---------------------------------------- */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }
    if (!primes) {
        primes_cap = 32;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
    }

    if (n == 0) return 0;  /* safety: offset is 1, but handle n==0 gracefully */

    /* extend cache until we have the requested term ----------------- */
    while (cache_len < (size_t)n) {
        /* test current for primality */
        if (is_prime_u64(current, primes, primes_len)) {
            /* store the prime for future tests */
            if (primes_len == primes_cap) grow_primes(&primes, &primes_cap, primes_len + 1);
            primes[primes_len++] = current;

            /* check for a twin pair with current + 2 */
            if (is_prime_u64(current + 2, primes, primes_len)) {
                seq_t rev_p = rev(current);                 /* rev returns seq_t */
                seq_t cur_seq = (seq_t)current;
                seq_t diff = (cur_seq > rev_p) ? (cur_seq - rev_p)
                                               : (rev_p - cur_seq);

                if (cache_len == cache_cap) grow_seq_cache(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = diff;
            }
        }
        current += 2;     /* only odd numbers needed */
    }

    return cache[n - 1];
}

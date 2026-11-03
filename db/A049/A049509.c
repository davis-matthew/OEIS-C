/*
 * A049509  Numbers k such that prime(k) == 7 (mod 10).
 * https://oeis.org/A049509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = index of the n‑th prime that is congruent to 7 modulo 10
   (computed by sieve) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence is defined from offset 1 */

    static seq_t *cache = NULL;     /* cached indices of suitable primes */
    static size_t cache_len = 0;    /* number of indices stored in cache   */
    static idx_t limit = 0;         /* upper bound used for current sieve  */

    /* Build more indices until we have at least n of them */
    while ((idx_t)cache_len < n) {
        /* Increase sieve limit (start with 10, then double) */
        if (limit == 0) limit = 10;
        else limit <<= 1;            /* equivalent to limit *= 2 */

        /* Generate all primes up to the current limit */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto(limit, &primes, &prime_cnt);

        /* Count primes ≡ 7 (mod 10) and store their 1‑based indices */
        seq_t *new_cache = mem_new(seq_t, prime_cnt);
        size_t seq_cnt = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if ((primes[i] % 10) == 7) {
                new_cache[seq_cnt++] = (seq_t)(i + 1); /* index of prime */
            }
        }
        free(primes);                /* memory from build_primes_upto */

        /* Replace old cache with the new one */
        mem_free(cache);
        cache     = new_cache;
        cache_len = seq_cnt;          /* number of indices stored */
    }

    return cache[n - 1];                 /* 1‑based access into cache */
}

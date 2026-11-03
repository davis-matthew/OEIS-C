/*
 * A142039  Primes congruent to 11 mod 32.
 * https://oeis.org/A142039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 11 mod 32 (OEIS A142039)
 * Range:     u8 n <= 4, u16 n <= 410, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Persistent state across calls */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;
    static idx_t last_candidate = 11 - 32;  /* start below first candidate */

    /* Ensure we have space for at least n primes */
    ensure_cache(n, &primes, &primes_cap);

    /* Generate primes until we reach the required index */
    while (primes_cnt < (size_t)n) {
        last_candidate += 32;            /* next number ≡ 11 (mod 32) */
        if (last_candidate % 3 == 0)     /* skip obvious composite */
            continue;
        if (is_prime(last_candidate)) {
            primes[primes_cnt++] = (seq_t)last_candidate;
        }
    }

    return primes[n - 1];
}

/*
 * A215310  Primes congruent to {1, 2, 3, 4, 5} mod 11.
 * https://oeis.org/A215310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p such that p % 11 ∈ {1,2,3,4,5} by incremental
   search and caching. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple deterministic prime test for 64‑bit integers. */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (uint64_t i = 5; i <= n / i; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    static seq_t *cache = NULL;           /* stored terms    */
    static size_t cache_size = 0;         /* number stored   */
    static size_t cache_cap = 0;          /* allocated size  */
    static uint64_t last_checked = 1;     /* last integer tested */

    /* Extend cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        ++last_checked;                   /* candidate */

        /* Skip even numbers > 2 for speed. */
        if (last_checked > 2 && (last_checked & 1) == 0) continue;

        if (!is_prime_uint64(last_checked)) continue;

        uint64_t r = last_checked % 11ULL;
        if (r < 1 || r > 5) continue;     /* not in the required residue set */

        /* Grow cache if needed. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {                     /* allocation failed */
                free(cache);
                cache = NULL;
                cache_cap = 0;
                cache_size = 0;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}

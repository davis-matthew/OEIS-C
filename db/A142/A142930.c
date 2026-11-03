/*
 * A142930  Primes congruent to 11 mod 64.
 * https://oeis.org/A142930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 64 == 11, 1‑based indexing
 * Range:     u8 n <= 2, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Prime test utility (provided by the build). */
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *cache   = NULL;
    static size_t count   = 0;     /* number of primes stored */
    static size_t cap     = 0;     /* current capacity of cache */
    static uint64_t next  = 11;    /* next candidate congruent to 11 mod 64 */

    /* Lazy initialization of the cache. */
    if (cache == NULL) {
        cap   = 16;
        cache = mem_new(seq_t, cap);
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
    }

    /* Generate more primes until the requested one is reached. */
    while (count < (size_t)n) {
        if (is_prime((idx_t)next)) {
            if (count >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) return (seq_t)0;   /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count] = (seq_t)next;
            ++count;
        }
        next += 64;
    }

    return cache[(size_t)(n - 1)];
}

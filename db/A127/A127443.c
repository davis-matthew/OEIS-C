/*
 * A127443  Primes p such that 6p - 11 and 6p + 11 are also primes.
 * https://oeis.org/A127443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 6p−11 and 6p+11 are also
 *            primes
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated on demand by a simple incremental search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;   /* array of results */
    static idx_t cached = 0;      /* number of terms stored */
    static idx_t next_candidate = 3; /* next odd number to test */

    /* Expand cache when needed */
    if (cache == NULL) {
        cache = mem_new(seq_t, 128);
        if (!cache) return 0;   /* allocation failure, return 0 as sentinel */
        cached = 0;
    }

    /* Generate terms until we reach the requested index n */
    while (cached < n) {
        /* Test current candidate */
        if (is_prime_seq(next_candidate)) {
            uint64_t x = 6ull * (uint64_t)next_candidate - 11;
            uint64_t y = 6ull * (uint64_t)next_candidate + 11;
            if (is_prime_seq(x) && is_prime_seq(y)) {
                /* Ensure cache has space */
                /* grow by doubling if needed (this compared the count with
                   the address malloc(1) returned, so it never grew) */
                static size_t cache_cap = 0;
                if ((size_t)cached >= cache_cap) {
                    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) return 0; /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cached++] = (seq_t)next_candidate;
            }
        }
        /* Move to the next odd candidate */
        next_candidate += 2;
    }

    /* Return the requested term (index is 1‑based) */
    return cache[n - 1];
}

/*
 * A167119  Primes congruent to 2, 3, 5, 7 or 11 (mod 13).
 * https://oeis.org/A167119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime p such that p % 13 equals 2, 3, 5, 7 or 11. */
/* The sequence has offset 1 (a(1) is the first such prime). */
/* The function follows a simple sieve‑style search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* cached terms, 0‑based */
    static size_t cache_cap = 0;       /* current allocated capacity */
    static size_t cache_sz  = 0;       /* number of terms stored */
    static idx_t last_checked = 1;     /* last integer examined (1‑based) */

    /* Grow the cache until it contains at least n terms. */
    while (cache_sz < n) {
        ++last_checked;                /* next integer to test */

        if (is_prime(last_checked)) {  /* primality test (fast enough for n≤10000) */
            idx_t m = last_checked % 13ULL;
            if (m == 2 || m == 3 || m == 5 || m == 7 || m == 11) {
                /* ensure we have room in the cache */
                if (cache_sz == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* allocation failure – return 0 (unlikely for given limits) */
                        return (seq_t)0;
                    }
                    cache   = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_sz++] = (seq_t)last_checked;   /* store the prime */
            }
        }
    }

    return cache[n - 1];   /* n is 1‑based */
}

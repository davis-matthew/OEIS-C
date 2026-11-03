/*
 * A097434  Value of C in y = x^2 + 5x + C such that y is prime for all x = 0
 *          to 3.
 * https://oeis.org/A097434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9630 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097434: C such that x^2+5x+C is prime for x = 0,1,2,3
   (C, C+6, C+14 and C+24 are all prime). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;               /* safety; the sequence is 1‑based */

    /* static cache of found C values */
    static seq_t *cache = NULL;
    static size_t cache_count = 0;      /* number of terms stored */
    static size_t cache_cap   = 0;      /* capacity in elements */
    static uint64_t candidate = 5;      /* next odd to test – first term is 5 */

    /* grow cache as needed */
    if (cache_count >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;            /* unlikely: allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms up to index n */
    while (cache_count < n) {
        /* find next candidate satisfying the four prime checks */
        while (true) {
            if (is_prime(candidate) &&
                is_prime(candidate + 6) &&
                is_prime(candidate + 14) &&
                is_prime(candidate + 24))
                break;                /* good candidate found */
            candidate += 2;           /* test next odd number */
        }

        /* store the term (truncated to seq_t if necessary) */
        if (cache_count >= cache_cap) {       /* grow before each store (it grew once) */
            size_t new_cap = cache_cap ? cache_cap << 1 : 32;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_count++] = (seq_t)candidate;

        /* advance to next odd candidate for the following search */
        candidate += 2;
    }

    return cache[n - 1];
}

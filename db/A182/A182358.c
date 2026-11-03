/*
 * A182358  Numbers n for which the number of divisors of n is congruent to 2
 *          mod 4.
 * https://oeis.org/A182358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9317 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182358: numbers n for which tau(n) ≡ 2 (mod 4). */
/* The function generates the sequence on demand, caching results
   so subsequent calls are O(1).  It uses the helper
   divisor_count_u64 from ../utils/divisor_count_u64.h. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static state shared between calls. */
    static seq_t *cache = NULL;    /* array holding the sequence values */
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static uint64_t next_int = 2;  /* number to test next */

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Count divisors of the current integer. */
        uint64_t tau = num_divisors(next_int);
        if ((tau % 4) == 2) {
            /* Ensure enough space. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) return 0; /* reallocation fail */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_int;
        }
        ++next_int;
    }

    return cache[n - 1];
}

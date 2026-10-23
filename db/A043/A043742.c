/*
 * A043742  Numbers whose number of runs in the base-2 representation is
 *          congruent to 3 mod 7.
 * https://oeis.org/A043742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose number of runs in binary
   representation ≡ 3 (mod 7).  Computed by sequential search and
   memoization. */

#include <stdlib.h>

/* Function required by the build system. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t current_candidate = 0;

    if (n == 0)
        return (seq_t)0;                 /* undefined for offset 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        current_candidate++;            /* next integer to test */

        /* Count runs in binary representation of current_candidate. */
        seq_t x = current_candidate;
        int runs = 0;
        int prev = -1;
        while (x > (seq_t)0) {
            int bit = (int)(x & (seq_t)1);
            if (bit != prev) {
                runs++;
                prev = bit;
            }
            x >>= 1;
        }

        if ((runs % 7) == 3) {          /* satisfies the condition */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* In production code handle this appropriately. */
                    exit(EXIT_FAILURE);
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = current_candidate;
        }
    }

    return cache[n - 1];
}

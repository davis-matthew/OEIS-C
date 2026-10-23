/*
 * A155152  Numbers k such that 13*k^2 + 3*k + 1 is prime.
 * https://oeis.org/A155152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array, isPrimeGeneric, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that 13*k^2+3*k+1 is prime.
   a(1) = 1, a(2) = 2, a(3) = 3, …   */
/* The function searches for the next k, caches all found k's
   and returns the n‑th one in O(1) amortized time.    */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n) {
    /* cache of previously found k's */
    static seq_t *cache = NULL;
    static size_t size = 0;   /* number of cached elements     */
    static size_t cap  = 0;   /* allocated capacity            */
    static idx_t next_k = 1;  /* next k to test                */

    /* Grow the cache until it contains at least n elements */
    while (size < n) {
        seq_t k   = (seq_t)next_k;
        seq_t val = ((seq_t)13 * k * k) + ((seq_t)3 * k) + (seq_t)1;

        if (isPrimeGeneric(val)) {
            if (size == cap) {
                size_t new_cap = (cap == 0 ? 16 : cap * 2);

                /* Allocate a larger buffer and copy old values */
                seq_t *new_cache = seq_alloc_new_array(new_cap);
                if (!new_cache) abort();          /* allocation failed */
                for (size_t i = 0; i < size; ++i)
                    new_cache[i] = cache[i];

                /* Free the old buffer (if any) */
                if (cache)
                    free_seq_array(cache);

                cache = new_cache;
                cap   = new_cap;
            }

            cache[size++] = k;
        }

        next_k++;
    }

    return cache[n - 1];
}

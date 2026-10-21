/*
 * A060378  Even-odd sieve.
 * https://oeis.org/A060378
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is obtained by starting with the even numbers 2,4,6,... and
 *            * then repeatedly striking out every 3rd, 5th, 7th, 9th, ...
 *            element of the * current list (with counting based on the
 *            current list after each strike). * The first term printed by the
 *            OEIS (a(2)) is 2. * Method: generate a sufficien...
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* n >= 2.  The required number of survivor terms is n-1. */
    static seq_t *cache = NULL;     /* array of surviving numbers   */
    static size_t cache_len = 0;    /* number of terms stored      */

    if (n < 2) {
        /* Undefined for n < 2; return 0 as a placeholder. */
        return (seq_t)0;
    }

    /* Need at least (n-1) terms stored. */
    size_t required = (size_t)(n - 1);
    if (cache_len < required) {
        /* Recompute the sequence up to the new required length. */
        size_t limit = required * 3;          /* initial guess for evens */
        seq_t *evens = NULL;
        size_t evens_len = 0;

        while (1) {
            /* Allocate array for the first 'limit' even numbers */
            evens = (seq_t *)calloc(limit, sizeof(seq_t));
            if (!evens) abort();              /* allocation failure */

            for (size_t i = 1; i <= limit; ++i) {
                evens[i - 1] = (seq_t)i * (seq_t)2;
            }
            evens_len = limit;

            /* Apply the odd-step sieves in place */
            for (idx_t step = 3; step <= (idx_t)evens_len; step += 2) {
                size_t w = 0;
                for (size_t i = 0; i < evens_len; ++i) {
                    if (((i + 1) % step) != 0) {
                        evens[w++] = evens[i];
                    }
                }
                evens_len = w;
            }

            if (evens_len >= required) {
                /* We have enough terms.  Shrink the array to the
                 * required size and store in the cache. */
                seq_t *new_cache = (seq_t *)realloc(evens,
                                                    required * sizeof(seq_t));
                if (!new_cache) new_cache = evens;   /* realloc may fail */
                free(cache);
                cache = new_cache;
                cache_len = required;
                break;
            }

            /* Not enough terms – free and try again with a larger limit */
            free(evens);
            limit *= 2;
        }
    }

    return cache[n - 2];   /* zero‑based offset: a(2) is cache[0] */
}

/*
 * A139244  a(0) = 4; a(n) = a(n-1)^2 - 1.
 * https://oeis.org/A139244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 4; a(n) = a(n-1)^2 - 1 for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"   /* seq_alloc_array */
#include "../utils/seq_free_array.h"    /* seq_free_array */

static seq_t *cache = NULL;  /* array of computed terms */
static size_t cache_len = 0; /* current number of terms stored */

/* Return the n‑th term of sequence A139244 */
seq_t a(idx_t n)
{
    /* Ensure space for (n+1) terms */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len)
    {
        /* Allocate a larger array */
        seq_t *new_cache = seq_alloc_array(needed);
        if (new_cache == NULL)
        {
            abort();                     /* allocation failure */
        }

        /* Copy existing terms */
        for (size_t i = 0; i < cache_len; ++i)
        {
            new_cache[i] = cache[i];
        }

        /* Compute new terms starting from the first missing index */
        for (size_t i = cache_len; i < needed; ++i)
        {
            if (i == 0)
            {
                new_cache[0] = (seq_t)4; /* a(0) = 4 */
            }
            else
            {
                seq_t prev = new_cache[i - 1];
                new_cache[i] = prev * prev - (seq_t)1;
            }
        }

        /* Release old storage */
        if (cache != NULL)
        {
            seq_free_array(cache);
        }

        /* Install the new cache */
        cache      = new_cache;
        cache_len  = needed;
    }

    return cache[(size_t)n];
}

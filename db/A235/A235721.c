/*
 * A235721  Squares which have one or more occurrences of exactly six
 *          different digits.
 * https://oeis.org/A235721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (kₙ)², where kₙ is the n‑th positive integer whose square
 *            contains exactly six distinct decimal digits
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so a(1) is the first term.  For
       n == 0 we return 0 as a defensive value. */
    if (n == 0) return 0;

    /* Cache of generated squares.  The array grows only as needed. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t  next_k      = 1;  /* next integer to test the square of */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        /* Compute the square of next_k as seq_t. */
        seq_t sq = (seq_t)next_k * (seq_t)next_k;

        /* Count distinct decimal digits of the square. */
        unsigned int freq[10] = {0};
        digit_frequencies(sq, freq);
        unsigned int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (freq[d]) ++distinct;

        /* Store the square if it has exactly six distinct digits. */
        if (distinct == 6)
        {
            if (cache_size == cache_cap)
            {
                /* Grow the cache: double the capacity, starting at 4. */
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – abort gracefully */
                {
                    free(cache);
                    cache = NULL;
                    cache_size = cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = sq;
        }

        ++next_k;
    }

    return cache[n - 1];
}

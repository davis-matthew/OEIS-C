/*
 * A162648  Locations of patterns 1001 or 0110 in the Thue-Morse sequence
 *          A010060.
 * https://oeis.org/A162648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A162648:  Locations of patterns 1001 or 0110 in the Thue‑Morse sequence.
   The implementation scans the Thue–Morse sequence for the desired 4‑bit patterns
   and memoises the discovered positions.  This yields O(n) time on the n‑th
   request and uses only a modest amount of memory.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Thue–Morse bit at index i: parity of popcount(i) */
static inline seq_t tm_bit(idx_t i)
{
    return (seq_t)__builtin_parityll((unsigned long long)i);
}

seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;   /* array of discovered indices */
    static size_t  cache_cap     = 0;      /* current allocated capacity */
    static size_t  cache_len     = 0;      /* number of entries stored */
    static idx_t   next_start    = 0;      /* index to start next search from */

    if (n == 0) return 0;                  /* sequence is 1‑indexed; this value is unused */

    /* Extend the cache until it contains at least n entries */
    while (cache_len < n)
    {
        /* Grow storage if necessary */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure – give up */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Search for the next matching position starting at next_start */
        idx_t i = next_start;
        for (;; ++i)
        {
            seq_t b0 = tm_bit(i);
            seq_t b1 = tm_bit(i + 1);
            seq_t b2 = tm_bit(i + 2);
            seq_t b3 = tm_bit(i + 3);
            if (b1 == b2 && b1 == (seq_t)(1 - b0) && b3 == b0)
                break;                  /* pattern found at position i */
        }

        /* Store the found position and prepare for the next search */
        cache[cache_len++] = (seq_t)i;
        next_start = i + 1;
    }

    return cache[n - 1];
}

/*
 * A062058  Numbers with 8 odd integers in their Collatz (or 3x+1) trajectory.
 * https://oeis.org/A062058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_odd_collatz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer whose Collatz (3x+1) trajectory contains exactly 8 odd integers (including the final 1). */
/* (The sequence is A062058.) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_odd_collatz.h"

/* Public sequence routine */
seq_t a(idx_t n)
{
    /* Cached list of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Next integer to test */
    static idx_t next_candidate = 1;

    /* Ensure the cache contains at least n terms */
    while (cache_len < n)
    {
        /* Find the next number with exactly 8 odd Collatz values */
        while (1)
        {
            if (count_odd_collatz((seq_t)next_candidate) == 8)
            {
                /* Grow cache if necessary */
                if (cache_len + 1 > cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();                /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                break;   /* move on to the next term requested */
            }
            ++next_candidate;
        }
        ++next_candidate;      /* avoid re‑testing the same number next time */
    }
    return cache[n - 1];
}

/*
 * A087144  Numbers with sum of digits not divisible by digital root.
 * https://oeis.org/A087144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer m such that the sum of the
 *            decimal digits of m is NOT divisible by the digital root of m
 *            (the reduction of m modulo 9, with 0 replaced by 9). Numbers
 *            with digital root 1 are automatically excluded
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digital_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digital_root.h"

/* Sum of decimal digits of n.  Works for all seq_t types that support
   the arithmetic operators used. */
static inline seq_t digit_sum(seq_t n)
{
    seq_t sum = 0;
    while (n > 0)
    {
        sum += n % (seq_t)10;
        n   /= (seq_t)10;
    }
    return sum;
}

/* Cache of computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 1;   /* next integer to test */

/* Resize the cache array when it becomes full. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, but return 0 */

    /* Lazy‑initialize cache. */
    if (cache_len == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n)
    {
        /* Grow cache capacity if needed. */
        if (cache_len >= cache_cap)
            grow_cache(cache_cap * 2);

        /* Test the candidate number. */
        int dr = digital_root(next_candidate);
        if (dr != 1)                  /* skip numbers with dr = 1 */
        {
            seq_t s = digit_sum(next_candidate);
            if (s % (seq_t)dr != 0)   /* condition satisfied */
                cache[cache_len++] = next_candidate;
        }
        next_candidate++;              /* move to the next integer */
    }
    return cache[n - 1];
}

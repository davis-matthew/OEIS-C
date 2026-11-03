/*
 * A062058  Numbers with 8 odd integers in their Collatz (or 3x+1) trajectory.
 * https://oeis.org/A062058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 923 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th integer whose Collatz (3x+1) trajectory contains exactly 8 odd integers (including the final 1). */
/* (The sequence is A062058.) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Counts the number of odd values in the Collatz trajectory of x,
   including the final 1. */
static int count_odd_collatz(seq_t x)
{
    int cnt = 0;
    while (1)
    {
        if (x == 1)
        {
            cnt++;     /* the final 1 is odd */
            break;
        }
        if ((x & 1) != 0)        /* x is odd */
        {
            cnt++;
            /* compute next value: 3*x + 1
               Use a 128‑bit intermediate for the 64‑bit build
               to avoid overflow when possible. */
#if defined(DATA_TYPE_UINT64_T)  /* pragma for 64‑bit build */
            __uint128_t tmp = (__uint128_t)3 * x + 1;
            x = (seq_t)tmp;    /* will be correct as long as tmp fits in 64 bits */
#else
            x = 3 * x + 1;
#endif
        }
        else /* x is even */
        {
            x >>= 1;            /* division by 2 */
        }
    }
    return cnt;
}

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
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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

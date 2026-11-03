/*
 * A008890  Aliquot sequence starting at 168.
 * https://oeis.org/A008890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A001065(a(n-1)), a(0) = 168
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 104, u64 n <= 175,
 *            u128 n <= 175, big any n
 * Validated: full, pass, 176 terms through n = 175 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Aliquot sequence starting at 168.  It terminates when 0 is reached. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;      /* array of stored terms */
    static size_t cache_cap = 0;     /* current allocated capacity */
    static size_t cache_len = 0;     /* number of terms stored */

    size_t needed = (size_t)n + 1;   /* we need terms up to index n */

    /* Grow the cache if we need more terms */
    if (cache_len < needed)
    {
        /* increase capacity */
        while (cache_cap < needed)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_cache = seq_alloc_new_array(new_cap);
            if (cache)
            {
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                seq_free_array(cache);
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        while (cache_len < needed)
        {
            if (cache_len == 0)          /* first term */
            {
                cache[0] = 168;
                cache_len = 1;
                continue;
            }

            seq_t prev = cache[cache_len - 1];
            seq_t next;

            if (prev == 0 || prev <= 1)  /* 0 or 1 has no proper divisors */
                next = 0;
            else
            {
                seq_t sum  = 1;               /* 1 is always a proper divisor */
                seq_t sqrt_n = floor_sqrt(prev);
                for (seq_t d = 2; d <= sqrt_n; ++d)
                {
                    if (prev % d == 0)
                    {
                        seq_t other = prev / d;
                        if (d == other)
                            sum += d;
                        else
                            sum += d + other;
                    }
                }
                next = sum;
            }

            cache[cache_len++] = next;
        }
    }

    return cache[n];
}

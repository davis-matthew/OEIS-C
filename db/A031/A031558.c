/*
 * A031558  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 60.
 * https://oeis.org/A031558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A031558:
   Numbers k such that the continued fraction of sqrt(k)
   has even period, and after deleting the last term of the period
   the central term equals 60. */
/* This implementation generates the sequence on demand,
   storing found terms in a cache so arbitrary call order is supported. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned values.
   Only the standard integer types are used. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1 << 62;
    while (bit > n) bit >>= 2;
    for (; bit != 0; bit >>= 2) {
        if (n >= r + bit) {
            n -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
    }
    return r;
}

/* Cache of values found so far.  The cache is 1‑indexed in logic
   but stored as a zero‑based array of seq_t. */
static seq_t *cache      = NULL;
static idx_t cache_count = 0;
static size_t cache_cap  = 0;

/* Next integer to test for k.  We start at 2 because 1 is a square. */
static idx_t next_k = 2;

seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (n > 0 && n <= cache_count)
        return cache[(idx_t)n - 1];

    /* Dynamic array for new values that will be appended to the cache. */
    while (cache_count < n)
    {
        idx_t D = next_k++;
        /* Skip perfect squares. */
        idx_t a0 = _isqrt_u64(D);
        if ((a0 * a0) == D)
            continue;

        /* Variables for continued fraction computation. */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;

        /* Period terms stored temporarily.  1024 is more than enough
           for all D up to the millions we will encounter for n <= ~10000. */
        seq_t period[1024];
        size_t plen = 0;

        /* Generate the periodic part. */
        for (;;)
        {
            /* Next term of the continued fraction. */
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0 + m) / d;

            /* Store the term.  The underlying type seq_t may be small;
               we simply cast, which suffices for the equality test. */
            period[plen++] = (seq_t)a_cur;

            /* The period ends when the term equals 2*a0. */
            if (a_cur == 2 * a0)
                break;
        }

        /* Check the required properties. */
        if ((plen % 2) == 0)         /* even period */
        {
            size_t mid_index = plen / 2;     /* 1‑based position of the central term */
            if (period[mid_index - 1] == (seq_t)60) /* central term equals 60 */
            {
                /* Ensure sufficient cache capacity. */
                if (cache_count == cache_cap)
                {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) /* fallback to aborting if memory allocation fails */
                        return (seq_t)0;
                    cache = tmp;
                }
                cache[cache_count++] = (seq_t)D;
            }
        }
    }

    return cache[(idx_t)n - 1];
}

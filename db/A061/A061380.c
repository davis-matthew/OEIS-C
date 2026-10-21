/*
 * A061380  Triangular numbers with product of digits also a triangular
 *          number.
 * https://oeis.org/A061380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular numbers with product of digits also a triangular number
   (OEIS A061380).  Sequence a(1)=0, a(2)=1, a(3)=3, … */

/* NOTE: This implementation uses only integer arithmetic and
   the provided floor_sqrt function.  It builds the sequence
   lazily on demand and caches previously computed terms so that
   calls in any order are efficient.  No external memory or IO
   is used. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/digit_product.h"

/* ------------------------------------------------------------ */
/*  Dynamic cache for the sequence values                        */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;         /* array of computed terms      */
static idx_t  cache_size = 0;       /* number of stored terms      */
static idx_t  cache_cap  = 0;       /* current allocated capacity  */
static idx_t  next_m     = 0;       /* next triangular index to test */

/* Grow the cache to at least 'needed' elements.              */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    cache_cap = cache_cap ? cache_cap * 2 : 16;
    if (cache_cap < needed)
        cache_cap = needed;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------ */
/*  Utilities                                                 */
/* ------------------------------------------------------------ */

/* Test whether x is a triangular number: exists t with x = t*(t+1)/2 */
static bool is_triangular(seq_t x)
{
    /* 8*x + 1 must be a perfect square with odd square root. */
    seq_t D = 8 * x + 1;
    seq_t s = floor_sqrt(D);
    return ((s * s) == D) && ((s & 1) == 1);
}

/* ------------------------------------------------------------ */
/*  Required function                                           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n<1, but return 0 */

    /* Grow cache to at least n terms */
    if (n > cache_size)
    {
        ensure_cache(n);

        while (cache_size < n)
        {
            /* Triangular number with index m = next_m */
            seq_t m  = (seq_t)next_m;
            seq_t Tm = (m * (m + (seq_t)1)) / (seq_t)2;

            /* product of digits of Tm */
            seq_t prod = digit_product(Tm);

            if (is_triangular(prod))
                cache[cache_size++] = Tm;   /* store the found term */

            next_m++;  /* advance to next triangular index */
        }
    }
    return cache[n - 1];
}

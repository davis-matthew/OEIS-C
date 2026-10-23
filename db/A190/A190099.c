/*
 * A190099  Number of 1:2:sqrt(5) proportioned triangles on an (n+1) X (n+1)
 *          grid.
 * https://oeis.org/A190099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A190099 – Number of 1:2:sqrt(5) proportioned triangles
 *  a(1)=0, a(2)=16, a(3)=64, a(4)=176, a(5)=384, a(6)=744, a(7)=1288,
 *          a(8)=2104, a(9)=3224, a(10)=4776, a(11)=6784, a(12)=9408,
 *          a(13)=12672, a(14)=16776
 *  For n>14: a(n)=a(n-1)+2*a(n-2)-2*a(n-3)+a(n-5)-3*a(n-6)+3*a(n-8)
 *                -a(n-9)+2*a(n-11)-2*a(n-12)-a(n-13)+a(n-14)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_last = 0;      /* last index stored in cache (>=14 after init) */
    static int    initialized = 0;     /* flag: base values loaded */

    /* allocate enough space for index n */
    size_t need = (size_t)n + 1;       /* keep index 0 unused for simplicity */
    if (cache_cap < need) {
        size_t new_cap = (cache_cap == 0 ? 16 : cache_cap);
        while (new_cap < need)
            new_cap <<= 1;             /* grow by factor 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        /* zero the new portion – works for arithmetic types and
           for arbitrary‑precision classes that can be assigned 0 */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values once */
    if (!initialized) {
        static const seq_t base[15] = {
            0,    /* index 0 (unused) */
            0,    /* a(1) */
            16,   /* a(2) */
            64,   /* a(3) */
            176,  /* a(4) */
            384,  /* a(5) */
            744,  /* a(6) */
            1288, /* a(7) */
            2104, /* a(8) */
            3224, /* a(9) */
            4776, /* a(10) */
            6784, /* a(11) */
            9408, /* a(12) */
            12672,/* a(13) */
            16776 /* a(14) */
        };
        for (size_t i = 1; i <= 14; ++i)
            cache[i] = base[i];
        cache_last = 14;
        initialized = 1;
    }

    /* compute terms up to n (if needed) */
    if ((size_t)n > cache_last) {
        for (size_t i = cache_last + 1; i <= (size_t)n; ++i) {
            cache[i] =
                cache[i - 1] +
                ((seq_t)2 * cache[i - 2]) -
                ((seq_t)2 * cache[i - 3]) +
                cache[i - 5] -
                ((seq_t)3 * cache[i - 6]) +
                ((seq_t)3 * cache[i - 8]) -
                cache[i - 9] +
                ((seq_t)2 * cache[i - 11]) -
                ((seq_t)2 * cache[i - 12]) -
                cache[i - 13] +
                cache[i - 14];
        }
        cache_last = n;
    }

    return cache[n];
}

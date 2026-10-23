/*
 * A192069  (A192249)/5.
 * https://oeis.org/A192069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192069 (OEIS):  a(n) = (A192249(n))/5.
   It satisfies the linear recurrence (for n ≥ 12):

        a(n) = -( d1·a(n-1) + d2·a(n-2) + … + d11·a(n-11) ),

   where the coefficients d_k are taken from the denominator of the
   generating function

        D(x) = (x-1)(x²+x-1)⁵
             = 1 – 6x + 10x² + 5x³ – 25x⁴ + 4x⁵ + 26x⁶ – 5x⁷ – 15x⁸
               + 0x⁹ + 4x¹⁰ + x¹¹

   Thus:
        d1 = –6,  d2 = 10,  d3 = 5,   d4 = –25, d5 = 4,
        d6 = 26,  d7 = –5,  d8 = –15, d9 = 0,  d10 = 4, d11 = 1.

   Initial values (offset 1):
        a(1)=0,  a(2)=1,  a(3)=4,  a(4)=18,  a(5)=60,
        a(6)=186, a(7)=522, a(8)=1380, a(9)=3459,
        a(10)=8321, a(11)=19332.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms – index 0 is unused, 1..11 are the real values. */
    static const seq_t init[12] = {
        0,        /* a(0) – not used */
        0,        /* a(1) */
        1,        /* a(2) */
        4,        /* a(3) */
        18,       /* a(4) */
        60,       /* a(5) */
        186,      /* a(6) */
        522,      /* a(7) */
        1380,     /* a(8) */
        3459,     /* a(9) */
        8321,     /* a(10) */
        19332     /* a(11) */
    };

    /* Recurrence coefficients (d1 … d11). */
    static const int d[12] = {
        0,   /* unused for k = 0 */
        -6,  /* d1 */
        10,  /* d2 */
        5,   /* d3 */
        -25, /* d4 */
        4,   /* d5 */
        26,  /* d6 */
        -5,  /* d7 */
        -15, /* d8 */
        0,   /* d9 */
        4,   /* d10 */
        1    /* d11 */
    };

    /* For small n we can answer directly. */
    if (n <= 11)
        return init[n];

    /* Cached values. */
    static seq_t *cache = NULL;          /* array of a(i) for i = 0 … cache_cap-1 */
    static idx_t cache_cap = 0;          /* current allocated size (≥ cache_max + 1) */
    static idx_t cache_max = 0;          /* largest index that has a valid value */

    /* First time: allocate enough space for at least the first 12 terms. */
    if (cache == NULL) {
        cache_cap = n + 1;
        if (cache_cap < 12)           /* safety, shouldn't happen because n>11 */
            cache_cap = 12;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;   /* allocation failed */
        /* Initialize the first 12 entries. */
        for (idx_t i = 0; i <= 11; ++i)
            cache[i] = init[i];
        cache_max = 11;
    } else if (n + 1 > cache_cap) {
        /* Grow the cache if necessary. */
        idx_t new_cap = n + 1;
        seq_t *p = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!p) return (seq_t)0;
        cache = p;
        cache_cap = new_cap;
    }

    /* Compute any missing terms beyond the current maximum. */
    if (n > cache_max) {
        /* Compute only from 12 onwards – lower indices are known. */
        idx_t start = cache_max < 11 ? 12 : cache_max + 1;
        for (idx_t i = start; i <= n; ++i) {
            /* a(i) = -(d1*a(i-1)+...+d11*a(i-11))  (d0 = 1, already involved in a(i)) */
            /* Compute sum of d_k*a(i-k).  Use unsigned arithmetic; subtraction at end gives sign. */
            idx_t k;
            seq_t sumpos = 0;   /* sum of terms with d_k >= 0 */
            seq_t sumneg = 0;   /* sum of terms with d_k < 0 */
            for (k = 1; k <= 11; ++k) {
                seq_t term = cache[i - k];
                int ck = d[k];
                unsigned long long mul = (ck >= 0) ? (unsigned long long)ck
                                                   : (unsigned long long)(-ck);
                term *= mul;                     /* term = |d_k| * a(i-k) */
                if (ck >= 0)
                    sumpos += term;
                else
                    sumneg += term;
            }
            cache[i] = sumneg - sumpos; /* a(i) = -(sum_{ck} d_k * a(i-k)) */
        }
        cache_max = n;
    }

    return cache[n];
}

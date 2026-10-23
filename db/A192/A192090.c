/*
 * A192090  Number of tatami tilings of a 4 X n grid (with monomers allowed).
 * https://oeis.org/A192090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 66, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192090 – number of tatami tilings of a 4×n grid (monomers allowed).
   The sequence satisfies the linear recurrence

   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4)+7*a(n-5)-7*a(n-6)+a(n-7)-a(n-8)-a(n-9)-a(n-10)+a(n-11)
           for all n ≥ 15

   with the initial values

   a(0) = 1  a(1) = 5   a(2) = 29   a(3) = 44   a(4) = 66
   a(5) = 126 a(6) = 238  a(7) = 490   a(8) = 922   a(9) = 1714
   a(10)=3306 a(11)=6246  a(12)=12102  a(13)=22994  a(14)=43682
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 15 terms */
    static const seq_t init[15] = {
        (seq_t)1,   (seq_t)5,   (seq_t)29,   (seq_t)44,   (seq_t)66,
        (seq_t)126, (seq_t)238, (seq_t)490,  (seq_t)922,  (seq_t)1714,
        (seq_t)3306,(seq_t)6246,(seq_t)12102,(seq_t)22994,(seq_t)43682
    };

    if (n < 15)
        return init[(size_t)n];

    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of initialized elements */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 15;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – caller will fail sensibly */
        for (size_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        cache_len = 15;
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_len) {
        if (cache_cap < (size_t)n + 1) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 15;
            if (new_cap < (size_t)n + 1)
                new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* compute missing terms */
        while (cache_len <= (size_t)n) {
            size_t idx = cache_len;          /* index of term to compute */
            seq_t val = cache[idx-1];        /* a(n-1) */
            val += cache[idx-2];             /* +a(n-2) */
            val += cache[idx-3];             /* +a(n-3) */
            val -= cache[idx-4];             /* -a(n-4) */
            val += (seq_t)7 * cache[idx-5];  /* +7*a(n-5) */
            val -= (seq_t)7 * cache[idx-6];  /* -7*a(n-6) */
            val += cache[idx-7];             /* +a(n-7) */
            val -= cache[idx-8];             /* -a(n-8) */
            val -= cache[idx-9];             /* -a(n-9) */
            val -= cache[idx-10];            /* -a(n-10) */
            val += cache[idx-11];            /* +a(n-11) */
            cache[idx] = val;
            ++cache_len;
        }
    }

    return cache[(size_t)n];
}

/*
 * A055384  Expansion of (1 + 4*x + 14*x^2 + 34*x^3 + 63*x^4 + 80*x^5 + 87*x^6
 *          + 68*x^7 + 42*x^8 + 20*x^9 + 7*x^10) / ((1 - x)*(1 - x^2)^2*(1 -
 *          x^3)^2*(1 - x^4)).
 * https://oeis.org/A055384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = a(n-1) + 2*a(n-2) -
 *            2*a(n-4) - 4*a(n-5) + a(n-6) + 3*a(n-7) + 3*a(n-8) + a(n-9) -
 *            4*a(n-10) - 2*a(n-11) + 2*a(n-13) + a(n-14) - a(n-15) with
 *            initial values a(0)=1,a(1)=5,a(2)=21,a(3)=65,a(4)=168,
 *            a(5)=364,a(6)=726,a(7)=1316,a(8)=2253,a(9)=3649, a(10)=56...
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, sizeof */

#if defined(__cplusplus)
  /* allow C++ compilation */
  #define ALLOC(k)  new seq_t[(k)]
  #define FREE(p)   delete[] (p)
#else
  #define ALLOC(k)  ((seq_t*)malloc((k)*sizeof(seq_t)))
  #define FREE(p)   free((p))
#endif

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cached_until = 0;/* first index not yet computed */

    /* initialise base terms if not already done */
    if (cache == NULL) {
        /* allocate capacity for at least 15 terms */
        const idx_t init_cap = 15;
        cache = ALLOC(init_cap);
        cache_cap = init_cap;
        /* seed initial values */
#if defined(__cplusplus)
        cache[0] = (seq_t)1;  cache[1] = (seq_t)5;  cache[2] = (seq_t)21;
        cache[3] = (seq_t)65; cache[4] = (seq_t)168; cache[5] = (seq_t)364;
        cache[6] = (seq_t)726;cache[7] = (seq_t)1316;cache[8] = (seq_t)2253;
        cache[9] = (seq_t)3649;cache[10] = (seq_t)5681;cache[11] = (seq_t)8505;
        cache[12] = (seq_t)12390;cache[13] = (seq_t)17544;cache[14] = (seq_t)24318;
#else
        cache[0] = (seq_t)1;  cache[1] = (seq_t)5;  cache[2] = (seq_t)21;
        cache[3] = (seq_t)65; cache[4] = (seq_t)168; cache[5] = (seq_t)364;
        cache[6] = (seq_t)726;cache[7] = (seq_t)1316;cache[8] = (seq_t)2253;
        cache[9] = (seq_t)3649;cache[10] = (seq_t)5681;cache[11] = (seq_t)8505;
        cache[12] = (seq_t)12390;cache[13] = (seq_t)17544;cache[14] = (seq_t)24318;
#endif
        cached_until = 15;   /* terms 0..14 are ready */
    }

    /* grow cache if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_cache = ALLOC(new_cap);
        for (idx_t i = 0; i < cached_until; ++i)
            new_cache[i] = cache[i];
        FREE(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cached_until; i <= n; ++i) {
        if (i <= 14) {
            /* already initialised */
            continue;
        }
        seq_t s = (seq_t)0;
        s +=  cache[i-1];
        s +=  (seq_t)2 * cache[i-2];
        s -=  (seq_t)2 * cache[i-4];
        s -=  (seq_t)4 * cache[i-5];
        s +=  cache[i-6];
        s +=  (seq_t)3 * cache[i-7];
        s +=  (seq_t)3 * cache[i-8];
        s +=  cache[i-9];
        s -=  (seq_t)4 * cache[i-10];
        s -=  (seq_t)2 * cache[i-11];
        s +=  (seq_t)2 * cache[i-13];
        s +=  cache[i-14];
        s -=  cache[i-15];
        cache[i] = s;
    }
    cached_until = n + 1;
    return cache[n];
}

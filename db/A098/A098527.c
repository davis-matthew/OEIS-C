/*
 * A098527  Expansion (1+x^3)/(1-x-x^7).
 * https://oeis.org/A098527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-7) Initial values: a(0)=1, a(1)=1, a(2)=1,
 *            a(3)=2, a(4)=2, a(5)=2, a(6)=2
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS sequence A098527.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated array capacity   */
    static size_t last = 0;       /* highest index computed so far */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                     /* initial growth factor */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure – unlikely */

        /* set the seven known initial terms */
        cache[0] = (seq_t)1;  cache[1] = (seq_t)1;  cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;  cache[4] = (seq_t)2;  cache[5] = (seq_t)2;
        cache[6] = (seq_t)2;
        last = 6;
    }

    /* expand cache to at least n if necessary */
    if ((size_t)n > last) {
        /* grow capacity if needed */
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t new_cap = cap;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing terms using the recurrence */
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-7];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}

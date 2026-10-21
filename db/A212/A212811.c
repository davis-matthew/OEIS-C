/*
 * A212811  Iterate the morphism 1->1123, 2->223, 3->133 starting with 1.
 * https://oeis.org/A212811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 156, u32 n <= 156, u64 n <= 156,
 *            u128 n <= 156, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Morphic sequence A212811: iterate the morphism
   1 -> 1123, 2 -> 223, 3 -> 133 starting with 1.
   This function returns the nth term (1-indexed). */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but safety */

    /* static cache of computed prefix */
    static seq_t *cache = NULL;
    static size_t cachesize = 0;
    static size_t cachecap   = 0;

    /* initialise with the starting symbol '1' */
    if (cachesize == 0) {
        cachecap   = 1;
        cache      = (seq_t *)malloc(cachecap * sizeof(seq_t));
        if (!cache) abort();
        cache[0]    = (seq_t)1;
        cachesize   = 1;
    }

    /* expand until we have at least n terms */
    while (cachesize < n) {
        size_t oldlen = cachesize;
        size_t newcap = oldlen * 4 + 1;  /* maximum possible new length + safety */
        seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newarr) abort();

        size_t newlen = 0;
        for (size_t i = 0; i < oldlen; ++i) {
            seq_t val = cache[i];
            if (val == (seq_t)1) {
                newarr[newlen++] = (seq_t)1;
                newarr[newlen++] = (seq_t)1;
                newarr[newlen++] = (seq_t)2;
                newarr[newlen++] = (seq_t)3;
            } else if (val == (seq_t)2) {
                newarr[newlen++] = (seq_t)2;
                newarr[newlen++] = (seq_t)2;
                newarr[newlen++] = (seq_t)3;
            } else if (val == (seq_t)3) {
                newarr[newlen++] = (seq_t)1;
                newarr[newlen++] = (seq_t)3;
                newarr[newlen++] = (seq_t)3;
            }
        }

        free(cache);
        cache      = newarr;
        cachesize  = newlen;
        cachecap   = newcap;
    }

    return cache[n-1];
}

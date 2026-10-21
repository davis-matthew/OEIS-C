/*
 * A234589  Expansion of g.f.: (1+x^6+x^7)/(1-2*x+x^6-x^7-x^8).
 * https://oeis.org/A234589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32, a(6)=64, a(7)=128
   a(n) = 2*a(n-1) - a(n-6) + a(n-7) + a(n-8)   for n >= 8 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;          /* current allocated size (number of entries) */
    static idx_t last  = 0;          /* highest index computed so far */

    /* initialise cache with first 8 terms */
    if (cap == 0) {
        cap = 8;
        cache = (seq_t*)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) abort();         /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)16;
        cache[5] = (seq_t)32;
        cache[6] = (seq_t)64;
        cache[7] = (seq_t)128;
        last  = 7;
    }

    /* grow cache if needed */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap += newcap;   /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* compute new entries */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)2 * cache[i-1];
        if (i >= 6) term -= cache[i-6];
        if (i >= 7) term += cache[i-7];
        if (i >= 8) term += cache[i-8];
        cache[i] = term;
    }
    if (n > last) last = n;

    return cache[n];
}

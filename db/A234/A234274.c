/*
 * A234274  Expansion of g.f. (1+x)*(1+x+x^2+x^3+x^4)/(1-6*x-6*x^2-6*x^3+x^4).
 * https://oeis.org/A234274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+6*a(n-2)+6*a(n-3)-a(n-4) for n≥6, with a(0)=1,
 *            a(1)=8, a(2)=56, a(3)=392, a(4)=2737, a(5)=19103
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t last  = 0;

    if (cache == NULL) {
        /* initialise first six terms */
        cap = 6;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)392;
        cache[4] = (seq_t)2737;
        cache[5] = (seq_t)19103;
        last = 5;
    }

    if ((size_t)n <= last) {
        return cache[(size_t)n];
    }

    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap ? cap : 1;
        while (need > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        /* zero the new part so any uncomputed entry is 0 */
        memset(tmp + cap, 0, (newcap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = 6 * cache[i-1] + 6 * cache[i-2] + 6 * cache[i-3] - cache[i-4];
    }
    last = (size_t)n;
    return cache[(size_t)n];
}

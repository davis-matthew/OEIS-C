/*
 * A074535  a(n) = 2^n + 4^n + 8^n.
 * https://oeis.org/A074535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 4^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;
    static seq_t p2 = (seq_t)1, p4 = (seq_t)1, p8 = (seq_t)1;

    /* first call – initialise cache for a(0) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* out‑of‑memory guard */
        cache[0] = p2 + p4 + p8;                  /* a(0) = 3 */
        last = 0;
    }

    /* extend computation if the requested index is beyond what we have */
    if (n > last) {
        /* grow the cache array */
        size_t new_cap = (n + 1) * 2;
        if (new_cap < (n + 1)) new_cap = n + 1;   /* avoid overflow */
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* out‑of‑memory guard */
            cache = tmp;
            cap = new_cap;
        }

        /* compute successive powers of 2, 4 and 8 */
        for (idx_t i = last + 1; i <= n; ++i) {
            p2 = p2 * (seq_t)2;
            p4 = p4 * (seq_t)4;
            p8 = p8 * (seq_t)8;
            cache[i] = p2 + p4 + p8;
        }
        last = n;
    }

    /* if n <= last, the value is already in cache */
    return cache[n];
}

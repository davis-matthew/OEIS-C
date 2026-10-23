/*
 * A191596  Expansion of (1+x)^4/(1-x)^7.
 * https://oeis.org/A191596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)
 *            Initial values: a(0)=1, a(1)=11, a(2)=62, a(3)=242, a(4)=743,
 *            a(5)=1925, a(6)=4396 Computed via a memoized recurrence
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* memoized sequence values */
    static size_t used = 0;          /* number of cached terms (next free index) */
    static size_t cap = 0;           /* allocated capacity of cache */
    static const seq_t base[7] = { (seq_t)1, (seq_t)11, (seq_t)62,
                                   (seq_t)242, (seq_t)743, (seq_t)1925,
                                   (seq_t)4396 };

    /* Grow or create the cache if necessary */
    if (n >= used) {
        if (cache == NULL) {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        }
        while (used <= n) {
            seq_t val;
            if (used < 7) {
                val = base[used];
            } else {
                /* Compute a(used) from the 7‑term recurrence */
                seq_t pos = (seq_t)0;
                seq_t neg = (seq_t)0;
                pos += cache[used-1] * (seq_t)7;
                pos += cache[used-3] * (seq_t)35;
                pos += cache[used-5] * (seq_t)21;
                pos += cache[used-7] * (seq_t)1;
                neg += cache[used-2] * (seq_t)21;
                neg += cache[used-4] * (seq_t)35;
                neg += cache[used-6] * (seq_t)7;
                val = pos - neg;
            }
            if (used == cap) {
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[used] = val;
            used++;
        }
    }
    return cache[n];
}

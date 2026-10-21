/*
 * A093199  Number of 4 X 4 magic squares with line sum n.
 * https://oeis.org/A093199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 12*a(n-3) + 17*a(n-4) + 8*a(n-5) -
 *            28*a(n-6) + 8*a(n-7) + 17*a(n-8) - 12*a(n-9) - 2*a(n-10) +
 *            4*a(n-11) - a(n-12) a(0)=1, a(1)=8, a(2)=48, a(3)=200, a(4)=675,
 *            a(5)=1904, a(6)=4736, a(7)=10608, a(8)=21925, a(9)=42328,
 *            a(10)=77328, a(11)=134680
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t allocSize = 0;   /* number of allocated slots */
    static idx_t computed = 0;    /* number of terms that have been filled */

    /* first 12 terms (index 0…11) */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)8,  (seq_t)48,  (seq_t)200, (seq_t)675,
        (seq_t)1904, (seq_t)4736, (seq_t)10608, (seq_t)21925,
        (seq_t)42328, (seq_t)77328, (seq_t)134680
    };

    /* initialise cache */
    if (allocSize == 0) {
        allocSize = 12;
        cache = (seq_t *)malloc(allocSize * sizeof(seq_t));
        if (cache == NULL) exit(1);                /* allocation failure */
        for (idx_t i = 0; i < 12; ++i) cache[i] = init[i];
        computed = 12;
    }

    /* if n is already available in the cache, return it */
    if (n < computed) return cache[n];

    /* grow the storage if needed */
    if (n >= allocSize) {
        idx_t newSize = allocSize * 2;
        if (newSize <= n) newSize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (tmp == NULL) exit(1);                  /* allocation failure */
        cache = tmp;
        allocSize = newSize;
    }

    /* compute the remaining terms up to n (inclusive) */
    for (idx_t k = computed; k <= n; ++k) {
        /* use 128‑bit intermediate to avoid overflow when seq_t is 64 bit */
        unsigned __int128 pos = 0, neg = 0;

        pos += (unsigned __int128)cache[k-1] * 4;
        pos += (unsigned __int128)cache[k-4] * 17;
        pos += (unsigned __int128)cache[k-5] * 8;
        pos += (unsigned __int128)cache[k-7] * 8;
        pos += (unsigned __int128)cache[k-8] * 17;
        pos += (unsigned __int128)cache[k-11] * 4;

        neg += (unsigned __int128)cache[k-2] * 2;
        neg += (unsigned __int128)cache[k-3] * 12;
        neg += (unsigned __int128)cache[k-6] * 28;
        neg += (unsigned __int128)cache[k-9] * 12;
        neg += (unsigned __int128)cache[k-10] * 2;
        neg += (unsigned __int128)cache[k-12];      /* coefficient -1 */

        cache[k] = (seq_t)(pos - neg);
    }

    computed = n + 1;
    return cache[n];
}

/*
 * A202525  Number of (n+2) X 3 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-3) + a(n-5) - 7*a(n-6) - 3*a(n-7) - a(n-8)
 *            + 2*a(n-9) + a(n-10) Initial values: a(1)=86, a(2)=198,
 *            a(3)=417, a(4)=890, a(5)=1940, a(6)=4184, a(7)=8994, a(8)=19420,
 *            a(9)=41918, a(10)=90355
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 52, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* largest n computed so far (>=0, 0 unused) */
    static size_t capacity = 0;     /* allocated size of cache array (>=1) */

    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Initialize cache on first call */
    if (cache == NULL) {
        capacity = 64;                         /* arbitrary initial size */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;           /* allocation failure */
        /* zero out the array (0th index unused) */
        for (size_t i = 0; i < capacity; ++i) cache[i] = 0;

        /* set known initial terms (index 1..10) */
        cache[1]  = (seq_t)86;
        cache[2]  = (seq_t)198;
        cache[3]  = (seq_t)417;
        cache[4]  = (seq_t)890;
        cache[5]  = (seq_t)1940;
        cache[6]  = (seq_t)4184;
        cache[7]  = (seq_t)8994;
        cache[8]  = (seq_t)19420;
        cache[9]  = (seq_t)41918;
        cache[10] = (seq_t)90355;
        cache_size = 10;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        /* zero new elements (not strictly needed but safe) */
        for (size_t i = capacity; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        capacity = new_cap;
    }

    /* If the requested term is already computed, return it immediately */
    if ((size_t)n <= cache_size) return cache[n];

    /* Compute terms up to the requested index */
    size_t start = cache_size + 1;
    if (start < 11) start = 11;          /* recurrence needs terms up to n-10 */

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t val = 0;
        val = cache[i-1]                                  /* a(i-1) */
            + (seq_t)6 * cache[i-3]                       /* 6*a(i-3) */
            + cache[i-5]                                  /* a(i-5) */
            - (seq_t)7 * cache[i-6]                       /* -7*a(i-6) */
            - (seq_t)3 * cache[i-7]                       /* -3*a(i-7) */
            - cache[i-8]                                  /* -a(i-8) */
            + (seq_t)2 * cache[i-9]                       /* +2*a(i-9) */
            + cache[i-10];                               /* +a(i-10) */
        cache[i] = val;
    }
    cache_size = (size_t)n;
    return cache[n];
}

/*
 * A030118  a(0) = 1, a(1) = 1, a(n) = a(n-1) - a(n-2) + n.
 * https://oeis.org/A030118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)-a(n-2)+n for n≥2
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t stored   = 0;   /* number of elements currently cached */
    static idx_t capacity = 0;

    if (cache == NULL) {                      /* first call – initialise cache */
        capacity = 2;
        cache = (seq_t*)malloc((size_t)capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely, but keeps compiler happy */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        stored  = 2;
    }

    if (n < stored) {                         /* value already computed */
        return cache[n];
    }

    /* grow cache until it holds value for index n */
    while (stored <= n) {
        if (stored == capacity) {             /* double the backing array */
            capacity *= 2;
            seq_t *tmp = (seq_t*)realloc(cache,
                                         (size_t)capacity * sizeof(seq_t));
            if (!tmp) return (seq_t)0;        /* memory failure – not expected */
            cache = tmp;
        }
        cache[stored] = cache[stored-1] - cache[stored-2]
                         + (seq_t)stored;
        ++stored;
    }

    return cache[n];
}

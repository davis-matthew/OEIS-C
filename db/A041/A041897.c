/*
 * A041897  Denominators of continued fraction convergents to sqrt(470).
 * https://oeis.org/A041897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 3382*a(n-6) - a(n-12) (n > 11) Initial values: a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=25, a(4)=53, a(5)=78, a(6)=3329, a(7)=3407,
 *            a(8)=10143, a(9)=84551, a(10)=179245, a(11)=263796
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Assuming seq_t and idx_t are defined elsewhere */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated capacity */
    static size_t size  = 0;      /* number of terms stored */

    /* initial terms */
    static const seq_t init[12] = {
        (seq_t)1,        (seq_t)1,        (seq_t)3,         (seq_t)25,
        (seq_t)53,       (seq_t)78,       (seq_t)3329,     (seq_t)3407,
        (seq_t)10143,    (seq_t)84551,    (seq_t)179245,   (seq_t)263796
    };

    /* first call: create cache and fill initial values */
    if (cache == NULL) {
        /* allocate capacity at least 12, round up to power of two for growth */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        size = 12;
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
    }

    /* if requested index is already cached, return it */
    if (n < size) return cache[n];

    /* grow cache to accommodate up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute terms from current size up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = 3382*a(i-6) - a(i-12) for i > 11 */
        seq_t val = (seq_t)3382 * cache[i - 6];
        val -= cache[i - 12];
        cache[i] = val;
    }
    size = needed;

    return cache[n];
}

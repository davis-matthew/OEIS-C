/*
 * A077970  Expansion of 1/(1+x-2*x^2+2*x^3).
 * https://oeis.org/A077970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=3; a(n) = -a(n-1)+2*a(n-2)-2*a(n-3) for
 *            n≥3
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 26, i64 n <= 53, i128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Lazy static cache for the sequence values. */
    static seq_t *cache = NULL;
    static idx_t cap  = 0;
    static idx_t used = 0;

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cap = 4;                     /* start with a small capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out‑of‑memory guard */
        cache[0] = (seq_t) 1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t) 3;
        used = 3;
    }

    /* Return cached value if already computed. */
    if (n < used)
        return cache[n];

    /* Grow cache as needed. */
    while (used <= n) {
        /* Ensure capacity for the new element. */
        if (used >= cap) {
            cap *= 2;
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }

        /* Recurrence: a(k) = -a(k-1) + 2*a(k-2) - 2*a(k-3) */
        seq_t val = -cache[used - 1]
                    + (seq_t)2 * cache[used - 2]
                    - (seq_t)2 * cache[used - 3];
        cache[used++] = val;
    }
    return cache[n];
}

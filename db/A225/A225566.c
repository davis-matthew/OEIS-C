/*
 * A225566  The set of magic numbers for an idealized harmonic oscillator
 *          atomic nucleus with a biaxially deformed prolate ellipsoid shape
 *          and an oscillator ratio of 3:1.
 * https://oeis.org/A225566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 *            for n > 8 Initial terms: a(1)=2, a(2)=4, a(3)=6, a(4)=12,
 *            a(5)=18, a(6)=24, a(7)=36, a(8)=48 Generated from:
 *            2*x/((1-x)^4*(1+x+x^2)^2) Cache on demand; O(n) per call
 * Range:     u8 n <= 16, u16 n <= 117, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Off‑by‑zero: a(1) is first element; index 0 unused */
    if (n < 1) return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* last computed index (>=0) */
    static idx_t capacity = 0;     /* allocated slots (>=1) */

    /* First call: allocate and initialize first 8 terms. */
    if (cache == NULL) {
        capacity = (n > 8 ? n + 1 : 9);          /* slots 0..capacity-1 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;     /* allocation failure */

        /* Initialize 1..8 */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)12;
        cache[5] = (seq_t)18;
        cache[6] = (seq_t)24;
        cache[7] = (seq_t)36;
        cache[8] = (seq_t)48;
        cache_size = 8;
    }

    /* Extend allocation if larger argument arrives */
    if (n >= capacity) {
        capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            capacity = 0;
            cache_size = 0;
            return (seq_t)0; /* out of memory */
        }
        cache = tmp;
    }

    /* Compute missing terms up to n */
    while (cache_size < n) {
        idx_t i = cache_size + 1;
        /* Terms 1..8 already set; recurrence for i>=9 */
        if (i > 8) {
            seq_t val = 2 * cache[i-1]
                        - cache[i-2]
                        + 2 * cache[i-3]
                        - 4 * cache[i-4]
                        + 2 * cache[i-5]
                        - cache[i-6]
                        + 2 * cache[i-7]
                        - cache[i-8];
            cache[i] = val;
        }
        cache_size = i;
    }

    return cache[n];
}

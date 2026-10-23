/*
 * A041416  Numerators of continued fraction convergents to sqrt(223).
 * https://oeis.org/A041416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 448*a(n-4) - a(n-8) for n >= 8 a(0) = 14, a(1) = 15, a(2)
 *            = 209, a(3) = 224 a(4) = 6481, a(5) = 6705, a(6) = 93646, a(7) =
 *            100351
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t allocated = 0;      /* size of allocated array */
    static idx_t computed = 0;       /* number of terms computed (next free index) */

    static const seq_t base[8] = {
        14, 15, 209, 224, 6481, 6705, 93646, 100351
    };

    idx_t i;

    /* allocate initial storage */
    if (cache == NULL) {
        allocated = 8;  /* at least needed for the initial 8 terms */
        cache = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!cache) return 0;   /* memory allocation failed */
    }

    /* compute or fill the base terms if not done yet */
    if (computed == 0) {
        for (i = 0; i < 8; ++i)
            cache[i] = base[i];
        computed = 8;   /* up to index 7 are now valid */
    }

    /* if requested term already computed, return it */
    if (n < computed)
        return cache[n];

    /* ensure we have enough space for up to n */
    if (n + 1 > allocated) {
        size_t new_alloc = allocated * 2;
        if (new_alloc < n + 1)
            new_alloc = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        allocated = new_alloc;
    }

    /* compute terms from the first missing up to n */
    for (i = computed; i <= n; ++i) {
        if (i < 8) {
            /* base values already filled above, but handle safety */
            cache[i] = base[i];
        } else {
            cache[i] = cache[i - 4] * (seq_t)448 - cache[i - 8];
        }
    }

    computed = n + 1;   /* update number of computed terms */
    return cache[n];
}

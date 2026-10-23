/*
 * A042509  Denominators of continued fraction convergents to sqrt(782).
 * https://oeis.org/A042509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1566*a(n-4) - a(n-8) for n >= 8 * Initial values: * a(0)
 *            = 1, a(1) = 1, a(2) = 27, a(3) = 28, * a(4) = 1539, a(5) = 1567,
 *            a(6) = 42281, a(7) = 43848 * * Computes the denominators of
 *            continued‑fraction convergents to sqrt(782). * * Structure: *
 *            Static cache holds all previously computed val...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n) {
    /* Static cache and state variables */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_cached = 0;

    /* Initialisation: allocate cache and store first 8 terms */
    if (cache == NULL) {
        cap = 8;                                 /* reserve space for 0..7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;  cache[1] = 1;  cache[2] = 27;   cache[3] = 28;
        cache[4] = 1539; cache[5] = 1567; cache[6] = 42281; cache[7] = 43848;
        max_cached = 7;
    }

    /* If we already have the value, just return it */
    if (n <= max_cached) {
        return cache[(size_t)n];
    }

    /* Ensure the cache is large enough for index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; in a competitive setting we could abort.
               Here we simply return 0 on error. */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute values from max_cached+1 up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 7) {
            /* Terms 0..7 are already set during initialisation */
            continue;
        }
        /* a(i) = 1566 * a(i-4) - a(i-8) */
        cache[(size_t)i] =
            (seq_t)1566 * cache[(size_t)(i - 4)] - cache[(size_t)(i - 8)];
    }
    max_cached = n;

    return cache[(size_t)n];
}

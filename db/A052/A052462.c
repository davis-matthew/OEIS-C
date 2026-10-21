/*
 * A052462  a(n) is the minimal positive integral solution k to 24*k == 1 (mod
 *          5^n).
 * https://oeis.org/A052462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 25*a(n-2) - 25*a(n-3) (n ≥ 4), with a(1) = 4,
 *            a(2) = 24, a(3) = 99
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed values */
    static seq_t *cache = NULL;      /* array of computed a(i), 1‑based index → 0‑based store */
    static idx_t cache_size = 0;     /* number of computed elements */

    /* initialise the first three terms */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory – unlikely in the test environment */
        cache[0] = (seq_t)4;   /* a(1) */
        cache[1] = (seq_t)24;  /* a(2) */
        cache[2] = (seq_t)99;  /* a(3) */
    }

    /* extend cache if needed */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;

        /* compute missing terms using the recurrence */
        /* i is 0‑based index corresponding to a(i+1) */
        for (idx_t i = cache_size; i < n; ++i) {
            seq_t val = cache[i-1]                    /* a(i)     */
                      + ((seq_t)25) * cache[i-2]      /* 25*a(i-1)*/
                      - ((seq_t)25) * cache[i-3];     /* 25*a(i-2)*/
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n-1]; /* return a(n) */
}

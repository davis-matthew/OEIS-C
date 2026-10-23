/*
 * A159831  Numerator of Hermite(n, 9/22).
 * https://oeis.org/A159831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11^n * Hermite(n, 9/22) Recurrence: a(0)=1, a(1)=9,
 *            a(n+1) = 9·a(n) – 242·n·a(n‑1) (for n ≥ 1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* current allocated size */
    
    /* ensure cache can hold indices up to n */
    if (cap <= n) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();    /* allocation failure */
        
        /* copy existing values */
        if (cache != NULL) {
            for (size_t i = 0; i < cap; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        
        /* initialise base cases if they are not already set */
        if (cap == 0) {
            cache[0] = 1;
            if (new_cap > 1) cache[1] = 9;
        }
        
        /* compute missing terms until n */
        for (size_t i = cap; i <= n; ++i) {
            if (i == 0) { cache[0] = 1; continue; }
            if (i == 1) { cache[1] = 9; continue; }
            seq_t t1 = 9 * cache[i-1];
            seq_t coeff = (seq_t)(242 * (i-1));
            seq_t t2 = coeff * cache[i-2];
            cache[i] = t1 - t2;
        }
        cap = new_cap;
    }
    return cache[n];
}

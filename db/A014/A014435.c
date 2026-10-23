/*
 * A014435  a(n) = Sum_{i=0..n-1} a(i)*a(n-i) with a(0)=1, a(1)=6.
 * https://oeis.org/A014435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A014435: a(n) = sum_{i=0}^{n-1} a(i)*a(n-1-i) - a(n-1)
 * with a(0) = 1, a(1) = 6.
 * 
 * The recurrence follows from the generating function
 * ( (1+x)-sqrt(1-2*x-23*x^2) ) / (2*x).
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static idx_t  cached = 0;     /* number of cached terms */

/* Compute a(n) */
seq_t a(idx_t n) {
    /* allocate or grow cache as needed */
    if (n >= cached) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        /* compute missing terms */
        for (idx_t i = cached; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)6;
            } else {
                seq_t sum = 0;
                for (idx_t j = 0; j < i; ++j) {
                    sum += cache[j] * cache[i - 1 - j];
                }
                cache[i] = sum - cache[i - 1];
            }
        }
        cached = n + 1;
    }
    return cache[n];
}

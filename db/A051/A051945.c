/*
 * A051945  a(n) = C(n)*(5*n+1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A051945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n)*(5*n+1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build environment. */

/* Static cache for Catalan numbers: C(i) is stored at catalan_cache[i]. */
static seq_t *catalan_cache = NULL;
static idx_t catalan_cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache for the required size if it does not yet exist. */
    if (catalan_cache_size == 0) {
        idx_t new_size = n + 1U;                     /* reserve space for indices 0..n */
        catalan_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!catalan_cache) return (seq_t)0;         /* memory exhaustion – return zero */
        catalan_cache[0] = (seq_t)1;                /* C(0) = 1 */
        catalan_cache_size = 1U;
    }

    /* Extend the cache up to n if needed. */
    if (n >= catalan_cache_size) {
        idx_t old_size = catalan_cache_size;
        idx_t new_size = n + 1U;
        seq_t *tmp = (seq_t*)realloc(catalan_cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                   /* memory exhaustion – return zero */
        catalan_cache = tmp;

        /* Compute missing Catalan numbers using the recurrence:
           C(0) = 1,
           C(i) = C(i-1) * 2*(2*(i-1)+1) / (i+1)   for i ≥ 1
         */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t prev = catalan_cache[i-1];
            seq_t top  = prev * (seq_t)(4 * (i-1) + 2);  /* 2*(2*(i-1)+1) = 4*(i-1)+2 */
            catalan_cache[i] = top / (seq_t)(i + 1);
        }
        catalan_cache_size = new_size;
    }

    /* Now produce a(n) = C(n) * (5*n + 1). */
    seq_t cat   = catalan_cache[n];
    seq_t factor = (seq_t)n * (seq_t)5 + (seq_t)1;
    return cat * factor;
}

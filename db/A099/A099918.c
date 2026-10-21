/*
 * A099918  A Chebyshev transform related to the 7th cyclotomic polynomial.
 * https://oeis.org/A099918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1; a(n) = b(n) – sum_{k=1..6} a(n-k), where b(0)=1,
 *            b(2)=2, b(4)=1 and b(n)=0 otherwise
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence from the generating function
   G(x) = (1+x²)² / (1 + x + … + x⁶).  */

#include <stdlib.h>
#include <stdint.h>   /* for uintptr_t if needed */

static seq_t *a_cache = NULL;   /* memoised values */
static idx_t a_cache_len = 0;   /* number of indices stored (0..len-1) */

seq_t a(idx_t n) {
    /* grow the cache to include index n */
    if (n >= a_cache_len) {
        /* reallocate */
        seq_t *new_cache = (seq_t*)realloc(a_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();               /* out of memory */
        a_cache = new_cache;

        /* compute new values */
        for (idx_t i = a_cache_len; i <= n; ++i) {
            seq_t b = 0;
            if (i == 0) b = 1;
            if (i == 2) b = 2;
            if (i == 4) b = 1;

            seq_t s = 0;
            for (idx_t k = 1; k <= 6; ++k) {
                if (i >= k) s += a_cache[i - k];
            }
            a_cache[i] = b - s;
        }
        a_cache_len = n + 1;
    }
    return a_cache[n];
}

/*
 * A099141  a(n) = 5^n * T(n,7/5) where T is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A099141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099141
   Recurrence: a(0)=1, a(1)=7, a(n)=14*a(n-1)-25*a(n-2) for n≥2   */
/* Computes a(n) for any n ≥ 0, memoizing results.  Supports both
   C and C++ builds with arbitrary integer type seq_t. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t size = 0;          /* number of computed entries */

    /* initial allocation of at least two entries */
    if (size == 0) {
        size_t init = (n < 2) ? 2 : (size_t)n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        size = init;
    }

    /* enlarge cache if needed and compute missing terms */
    if ((size_t)n + 1 > size) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failed */
        cache = tmp;

        /* start computing from the first index we haven't filled */
        for (size_t i = size; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)14 * cache[i-1] - (seq_t)25 * cache[i-2];
        }
        size = newsize;
    }

    return cache[(size_t)n];
}

/*
 * A226588  a(n) = c({1}^n), the Cantor tuple function c applied to an n-tuple
 *          of 1's.
 * https://oeis.org/A226588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+1)*(a(n-1)+2)/2 + 1 for n>1, a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation with a dynamic array allocated via realloc.
   The cast is required for a C++ build where seq_t may be a class type. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Protect against impossible indices; the sequence is defined for n>=0 only. */
    if (n >= SIZE_MAX) abort();

    size_t N = (size_t)n;          /* N is safe for array indexing up to about 10000 */

    /* Static cache and state */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* capacity of the cache array */
    static size_t mx    = 0;    /* highest computed index in cache (>=0) */

    /* Ensure the cache is large enough for N. */
    if (N + 1 > cap) {
        size_t new_cap = (cap ? cap * 2 : 1);
        while (new_cap <= N) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure is fatal */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the fixed base values.  Reinitialising them is harmless. */
    cache[0] = 0;
    if (N >= 1) cache[1] = 1;
    if (mx < 1) mx = 1;   /* at least index 1 is known after this point */

    /* Compute missing terms only, if required. */
    size_t start = (mx >= 2 ? mx + 1 : 2);
    if (N >= start) {
        for (size_t k = start; k <= N; ++k) {
            seq_t prev = cache[k - 1];
            cache[k] = (prev + 1) * (prev + 2) / 2 + 1;
        }
        mx = N;   /* update highest computed index */
    }

    return cache[N];
}

/*
 * A077995  Expansion of (1 - x)/(1 - 2*x - 2*x^2 - x^3).
 * https://oeis.org/A077995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3) with initial values a(0)=1,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms and its size. */
    static seq_t *cache         = NULL;   /* array of calculated terms   */
    static idx_t alloc_size    = 0;        /* capacity of the array       */
    static idx_t max_computed  = 0;        /* highest index that has been computed */

    /* ---------- initialise cache if necessary ---------- */
    if (alloc_size == 0) {
        /* Reserve space for the first three terms (a(0..2)). */
        alloc_size = 3;
        cache = (seq_t *)realloc(NULL, alloc_size * sizeof(seq_t));
        if (!cache) abort();

        /* initialise the three known terms.  Value 0 is used only as a
           marker for terms that have not yet been computed. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        max_computed = 2;
    }

    /* ---------- grow cache if we must store up to index n ---------- */
    if (alloc_size <= n) {
        idx_t new_size = alloc_size;
        while (new_size <= n) new_size <<= 1;     /* double until large enough */

        /* realloc works for both C and C++ builds; cast the void* result. */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();

        /* zero‑initialise the newly added slots (values will be computed later). */
        for (idx_t i = alloc_size; i < new_size; ++i) new_cache[i] = 0;

        cache      = new_cache;
        alloc_size = new_size;
    }

    /* ---------- compute any missing terms up to n ---------- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 0;
        /* recurrence uses a(i-1), a(i-2), a(i-3) for i >= 3 */
        if (i >= 3) {
            val  = cache[i-1];
            val  = val * 2   +   cache[i-2] * 2   +   cache[i-3];
        }
        cache[i] = val;
    }
    /* Update the highest computed index. */
    if (n > max_computed) max_computed = n;

    /* ---------- return the requested term ---------- */
    return cache[n];
}

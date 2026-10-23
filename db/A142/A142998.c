/*
 * A142998  a(0) = 0, a(1) = 1, a(n+1) = (2*n^2+2*n+21)*a(n) - n^4*a(n-1).
 * https://oeis.org/A142998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = (2*n^2+2*n+21)*a(n) - n^4*a(n-1) Initial values: a(0) =
 *            0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes term a(n) of OEIS A142998. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;     /* array of seq_t values */
    static size_t cap = 0;          /* allocated capacity */
    static bool init = false;       /* whether base terms have been set */
    static idx_t last = 0;          /* highest index computed so far */

    /* First call: initialise cache with a(0) and a(1) */
    if (!init) {
        /* Allocate space for at least two terms (n=1). */
        cap = 2;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();            /* Out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        last = 1;
        init = true;
    }

    /* If the requested term is already computed, return it. */
    if (n <= (idx_t)last)
        return cache[n];

    /* Ensure capacity for all terms up to n. */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* Out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms from last+1 up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* For a(i) we need n = i-1 in the recurrence. */
        seq_t tn = (seq_t)(i - 1);
        seq_t n2 = tn * tn;
        seq_t coeff = (seq_t)2 * n2 + (seq_t)2 * tn + (seq_t)21;
        seq_t term1 = coeff * cache[i - 1];
        seq_t n4 = n2 * n2;
        seq_t term2 = n4 * cache[i - 2];
        cache[i] = term1 - term2;
    }

    last = n;
    return cache[n];
}

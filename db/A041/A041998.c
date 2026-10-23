/*
 * A041998  Numerators of continued fraction convergents to sqrt(522).
 * https://oeis.org/A041998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent to √522. Continued
 *            fraction of √522 is [22; 1,5,1,1,4,1,1,5,1,44] (period 10).
 *            Recurrence: p₋₂ = 0, p₋₁ = 1, p₀ = 22, p₁ = 23, p_k =
 *            a_k·p_{k‑1} + p_{k‑2} for k ≥ 2, where a₀ = 22 and for k ≥ 1,
 *            a_k = periodic sequence {1,5,1,1,4,1,1,5,1,44}
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of computed numerators */
    static seq_t *cache = NULL;
    static size_t  cap = 0;
    static idx_t  computed = 0;  /* highest n that has been computed */

    /* initialise the cache on first call */
    if (!cache) {
        cap = 16;                           /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        memset(cache, 0, cap * sizeof(seq_t));

        /* first two convergents */
        cache[0] = 22;
        cache[1] = 23;
        computed = 1;
    }

    /* if the requested index is already in the cache, return it */
    if (n <= computed)
        return cache[n];

    /* grow the cache to hold up to index n */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;          /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* periodic part of the continued fraction of √522 */
    static const seq_t period_vals[10] = {1,5,1,1,4,1,1,5,1,44};
    static const idx_t period = 10;

    /* compute coefficients from the last computed up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t idx = (i - 1) % period;          /* index in period (i>=1) */
        cache[i] = period_vals[idx] * cache[i - 1] + cache[i - 2];
    }
    computed = n;

    return cache[n];
}

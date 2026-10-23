/*
 * A185552  1/3 the number of n X 2 0..2 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 7*a(n-2) + 8*a(n-3) - 8*a(n-5) for n≥6 Initial
 *            values: a(1) = 1 a(2) = 5 a(3) = 26 a(4) = 120 a(5) = 576
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t, NULL */
#include <stdint.h>     /* uint64_t, etc. */
#include <stdlib.h>     /* malloc, realloc, free */
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1) …   */
    static idx_t cache_cap = 0;      /* highest index that has been cached   */
    static idx_t last_computed = 0;  /* largest n that has been computed    */

    /* ---- initialization ------------------------------------------------- */
    if (!cache) {
        /* Need indices 0 … 5 (a(1)..a(5)).  Allocate 6 slots.          */
        cache_cap = 5;
        cache = seq_alloc_new_array(cache_cap + 1);   /* allocates 6 slots   */
        if (!cache) return (seq_t)0;                  /* memory allocation failure */

        /* base terms ( 0‑th entry unused ) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)26;
        cache[4] = (seq_t)120;
        cache[5] = (seq_t)576;
        last_computed = 5;
    }

    /* ---- enlarge cache if required ------------------------------------- */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 5;
        while (new_cap < n) new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---- compute missing terms using the recurrence ------------------- */
    while (last_computed < n) {
        ++last_computed;
        /* Recurrence is valid for last_computed ≥ 6. */
        seq_t val = (seq_t)3 * cache[last_computed-1]      /* a(n-1) */
                  + (seq_t)7 * cache[last_computed-2]      /* a(n-2) */
                  + (seq_t)8 * cache[last_computed-3]      /* a(n-3) */
                  - (seq_t)8 * cache[last_computed-5];     /* a(n-5) */
        cache[last_computed] = val;
    }

    return cache[n];
}

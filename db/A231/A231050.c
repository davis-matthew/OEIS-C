/*
 * A231050  Number of 2 X n 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4 and at least one element with value
 *          (x(i,j)-1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-3) + 6*a(n-4) - 4*a(n-5) for n>6 Initial
 *            values: a(1)=0, a(2)=2, a(3)=14, a(4)=32, a(5)=100, a(6)=428
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] holds a(i) for i ≥ 0 (cache[0] unused) */
static size_t cache_size = 0;    /* current allocated size of cache */

/* Compute a(n) for n ≥ 1 using memoised recurrence */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n < 1 */

    const size_t idx = (size_t)n;

    /* --- initialise cache with base values (once) --- */
    if (!cache) {
        cache_size = 7;                     /* indices 0..6 are allocated */
#ifdef __cplusplus
        cache = new seq_t[cache_size];
#else
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
#endif
        if (!cache) exit(1);
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)32;
        cache[5] = (seq_t)100;
        cache[6] = (seq_t)428;
    }

    /* --- extend the cache if needed --- */
    size_t old_size = cache_size;          /* keep first value to use in loop below */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;          /* need space up to index n */
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_size];
        /* copy existing values */
        for (size_t j = 0; j < cache_size; ++j) tmp[j] = cache[j];
        delete[] cache;                    /* free old array */
#else
        tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        /* realloc preserves the existing contents in the C case */
#endif
        if (!tmp) exit(1);
        cache = tmp;
        cache_size = new_size;
    }

    /* --- compute missing terms using the recurrence --- */
    for (size_t i = old_size; i <= idx; ++i) {
        if (i <= 6) continue;               /* base values already set */
        cache[i] = cache[i-1] * (seq_t)3
                 + cache[i-3] * (seq_t)8
                 + cache[i-4] * (seq_t)6
                 - cache[i-5] * (seq_t)4;
    }

    return cache[idx];
}

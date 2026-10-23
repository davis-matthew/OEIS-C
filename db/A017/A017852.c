/*
 * A017852  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10-x^11-x^12).
 * https://oeis.org/A017852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12) for n
 *            > 11, a(0)=1, a(1)=a(2)=a(3)=a(4)=a(5)=0 (number of compositions
 *            of n with parts 6…12)
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 106, u64 n <= 204,
 *            u128 n <= 399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, calloc, realloc */
#include <stddef.h>  /* size_t */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *memo = NULL;      /* cache array */
    static size_t cache_size = 0;   /* allocated size */
    static size_t max_computed = (size_t)-1; /* highest index computed */

    /* initialise cache with space for 13 entries (0..12) */
    if (memo == NULL) {
        cache_size = 13;
        memo = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (memo == NULL) return 0;  /* fallback: out of memory, return 0 */
        memo[0] = 1;                 /* base value a(0) */
        max_computed = 0;
    }

    /* if already computed up to n, return value */
    if (max_computed != (size_t)-1 && (size_t)n <= max_computed)
        return memo[(size_t)n];

    /* ensure the cache is large enough */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (new_mem == NULL) return 0; /* memory failure */
        /* zero the newly allocated part (except a(0) which is already 1) */
        for (size_t i = cache_size; i < new_size; ++i)
            new_mem[i] = 0;
        memo = new_mem;
        cache_size = new_size;
    }

    /* compute values from last computed index + 1 up to n */
    size_t start = (max_computed == (size_t)-1) ? 1 : max_computed + 1;
    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 0) {
            memo[i] = 1;                /* already set but keep for clarity */
        } else if (i <= 5) {
            memo[i] = 0;                /* a(1) … a(5) = 0 */
        } else {
            seq_t sum = 0;
            /* sum a(i-k) for k = 6 … 12 if i >= k */
            if (i >= 12) sum += memo[i - 12];
            if (i >= 11) sum += memo[i - 11];
            if (i >= 10) sum += memo[i - 10];
            if (i >= 9)  sum += memo[i - 9];
            if (i >= 8)  sum += memo[i - 8];
            if (i >= 7)  sum += memo[i - 7];
            if (i >= 6)  sum += memo[i - 6];
            memo[i] = sum;
        }
    }

    max_computed = (size_t)n;
    return memo[(size_t)n];
}

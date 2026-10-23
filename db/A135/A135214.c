/*
 * A135214  a(1)=1, a(n) = a(n-1) + n^5 if n odd, a(n) = a(n-1) + n^4 if n is
 *          even.
 * https://oeis.org/A135214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1; 
 *        a(n) = a(n-1) + n^5   if n is odd, 
 *               a(n-1) + n^4   if n is even
 *
 * The implementation memoizes the sequence in an expanding array.
 * It works for any build where seq_t is an unsigned integer type with
 * the usual arithmetic operators available.
 */

#include <stdlib.h>

/* Helper: compute base^exp using seq_t (exp ∈ {4,5}) */
static inline seq_t pow_exp(idx_t base, int exp) {
    seq_t result = 1;
    seq_t b = (seq_t)base;
    for (int i = 0; i < exp; ++i) {
        result *= b;
    }
    return result;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Static cache: indices 1..max_cached */
    static seq_t *cache = NULL;
    static idx_t max_cache = 0;   /* highest index computed */
    static size_t capacity = 0;   /* allocated size of cache array */

    if (n == 0) {
        /* Undefined for n < 1; return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Ensure cache exists and contains at least a[1] */
    if (max_cache == 0) {
        capacity = 2;               /* we need index 1; allocate 2 slots */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) return (seq_t)0; /* allocation failure guard */
        cache[1] = (seq_t)1;        /* a(1) = 1 */
        max_cache = 1;
    }

    /* If requested index already cached, return it directly */
    if (n <= max_cache) {
        return cache[n];
    }

    /* Expand cache if needed */
    if (n + 1 > capacity) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) return (seq_t)0;  /* realloc failure guard */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing terms iteratively */
    for (idx_t i = max_cache + 1; i <= n; ++i) {
        seq_t add = pow_exp(i, (i & 1ULL) ? 5 : 4); /* odd => 5, even => 4 */
        cache[i] = cache[i - 1] + add;
    }
    max_cache = n;

    return cache[n];
}

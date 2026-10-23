/*
 * A170093  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170093
 * Recurrence:  a(n) = 34·∑_{i=1}^{36} a(n-i)  – 595·a(n-37)  + c(n)
 * where c(n)=1 if n=0 or n=37, c(n)=2 if 1≤n≤36, c(n)=0 otherwise.
 * The recurrence holds for all n≥0 with a(k)=0 for k<0.
 * This implementation memoises all computed values so that every call
 * returns the correct term in O(n) time and O(n) space.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* uint64_t */

#ifdef __cplusplus
/* C++ code: use operator new/delete */
static seq_t* allocate_seq(size_t size)
{
    return new seq_t[size];
}
static void deallocate_seq(seq_t* ptr)
{
    delete[] ptr;
}
#else
/* C code: use malloc/realloc */
static seq_t* allocate_seq(size_t size)
{
    return (seq_t*)malloc(size * sizeof(seq_t));
}
static void deallocate_seq(seq_t* ptr)
{
    free(ptr);
}
#endif

/* cached values of the sequence */
static seq_t  *cache      = NULL;   /* array of computed terms      */
static size_t  cache_cap  = 0;      /* allocated capacity           */
static size_t  cache_len  = 0;      /* number of computed entries   */

/* ensure that cache has space for index n */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap > (size_t)n + 1) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;

    /* allocate new buffer and copy existing values */
    seq_t *new_cache = allocate_seq(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    /* zero the rest (not strictly needed if we only read after write) */
    for (size_t i = cache_len; i < new_cap; ++i) new_cache[i] = 0;
    /* free old buffer */
    deallocate_seq(cache);
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* main function implementing the sequence */
seq_t a(idx_t n)
{
    /* initialise cache lazily */
    if (cache_len == 0) {
        ensure_cache_capacity(0);
        cache[0] = (seq_t)1;   /* a(0)=1 */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* RHS constant term c(i) */
        seq_t c = 0;
        if (i == 0)          c = (seq_t)1;
        else if (i <= 36)    c = (seq_t)2;
        else if (i == 37)    c = (seq_t)1;
        /* sum of the 36 previous terms multiplied by 34 */
        seq_t sum = 0;
        for (int k = 1; k <= 36; ++k) {
            idx_t j = i - k;
            if (j < (idx_t)cache_len) sum += cache[j];
        }
        sum *= (seq_t)34;
        /* subtract 595·a(i-37) if applicable */
        seq_t term = sum;
        if (i >= 37) term -= (seq_t)595 * cache[i - 37];
        /* add the constant c(i) */
        term += c;
        /* store the new term */
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}

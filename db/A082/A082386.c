/*
 * A082386  Let S(0) = {1} and for k >= 1, let S(k) = {S(k-1), S(k-1), 2T},
 *          where T = sum of terms in S(k-1). Sequence is S(infinity).
 * https://oeis.org/A082386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A082386 - recursively duplicate a sequence and append twice its
 *            sum. The sequence is built as: S(0) = {1} T = sum(S(k-1)) S(k) =
 *            {S(k-1), S(k-1), 2*T} The infinite sequence a(n) is the
 *            concatenation of all S(k). The implementation lazily expands an
 *            internal cache until it contains at least n+1 el...
 * Range:     u8 n <= 61, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Grow an array of seq_t elements to at least 'need' capacity. */
static void grow_array(seq_t **arr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : *cap * 2;
        while (new_cap < need)
            new_cap *= 2;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Append a value to a dynamic seq_t array. */
static void push_value(seq_t **arr, size_t *size, size_t *cap, seq_t val) {
    grow_array(arr, cap, *size + 1);
    (*arr)[*size] = val;
    ++(*size);
}

/* Return the n-th term of A082386. */
seq_t a(idx_t n) {
    /* Static cache of generated terms. */
    static seq_t *cache           = NULL;
    static size_t cache_size      = 0;
    static size_t cache_capacity  = 0;
    static seq_t sum_prev         = 0;   /* sum of terms in the current S(k-1) */

    /* Initialisation on first call. */
    if (cache == NULL) {
        /* S(0) = {1} */
        push_value(&cache, &cache_size, &cache_capacity,
                   (seq_t)1);
        sum_prev = (seq_t)1;
    }

    /* Expand until we have at least n+1 terms. */
    while (cache_size <= (size_t)n) {
        size_t old_size = cache_size;           /* length of S(k-1)      */
        seq_t T = sum_prev;                     /* sum of S(k-1)        */
        seq_t new_val = (seq_t)((uint64_t)T * 2ULL);  /* 2*T */

        /* New length: 2*old_size + 1   */
        size_t new_size_needed = old_size * 2 + 1;
        grow_array(&cache, &cache_capacity, new_size_needed);

        /* Copy old array twice into the new space. */
        for (size_t i = 0; i < old_size; ++i)
            cache[i] = cache[i];                 /* first copy (unchanged) */
        for (size_t i = 0; i < old_size; ++i)
            cache[old_size + i] = cache[i];      /* second copy      */

        /* Append the new term 2*T. */
        cache[new_size_needed - 1] = new_val;
        cache_size = new_size_needed;

        /* Update the sum for the next iteration: new sum = 4*old_sum. */
        sum_prev = (seq_t)((uint64_t)T * 4ULL);
    }

    return cache[(size_t)n];
}

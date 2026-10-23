/*
 * A193605  Triangle: (row n) = partial sums of partial sums of row n of
 *          Pascal's triangle.
 * https://oeis.org/A193605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,c) where 
 *     r = ⌊(√(8n+1)-1)/2⌋,
 *     c = n - r(r+1)/2,
 * and T(r,c) = Σ_{i=0..c} (c-i+1)·binom(r,i). 
 * The triangle rows are built iteratively by recurrence on binomial
 * coefficients, then by partial summation.
 */

#include <stddef.h>
#include <stdlib.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Build one triangle row and append its values to the cache. */
static void build_row(idx_t r,
                      seq_t **cache, idx_t *cache_size, idx_t *cache_capacity,
                      seq_t **prev_binom, idx_t *prev_len)
{
    /* binomial row for index r */
    seq_t *curr_binom = alloc_seq_array((size_t)r + 1);
    curr_binom[0] = 1;
    if (r > 0) {
        curr_binom[r] = 1;
        for (idx_t k = 1; k < r; ++k) {
            /* curr_binom[k] = prev_binom[k-1] + prev_binom[k] */
            curr_binom[k] = (*prev_binom)[k - 1] + (*prev_binom)[k];
        }
    }

    /* partial sums of binomials (S) and their cumulative sums (T) */
    seq_t *S = alloc_seq_array((size_t)r + 1);
    seq_t *T = alloc_seq_array((size_t)r + 1);
    S[0] = curr_binom[0];
    T[0] = S[0];
    for (idx_t k = 1; k <= r; ++k) {
        S[k] = S[k - 1] + curr_binom[k];
        T[k] = T[k - 1] + S[k];
    }

    /* expand cache if necessary */
    size_t needed = (size_t)(*cache_size) + (size_t)(r + 1);
    if (needed > *cache_capacity) {
        size_t new_cap = (*cache_capacity) ? (*cache_capacity) * 2 : 4;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
        *cache = tmp;
        *cache_capacity = new_cap;
    }

    /* append the T values */
    for (idx_t k = 0; k <= r; ++k) {
        (*cache)[(*cache_size) + k] = T[k];
    }
    *cache_size += (r + 1);

    /* clean up temporary arrays */
    free_seq_array(S);
    free_seq_array(T);
    free_seq_array(*prev_binom);
    *prev_binom = curr_binom;
    *prev_len = r + 1;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Static data for lazy, incremental construction */
    static seq_t *cache = NULL;          /* flattened triangle values   */
    static idx_t cache_capacity = 0;     /* allocated cache capacity    */
    static idx_t cache_size = 0;         /* number of elements cached   */
    static idx_t max_row = 0;            /* largest fully built row      */
    static seq_t *prev_binom = NULL;     /* previous row of binomials   */
    static idx_t prev_len = 0;           /* length of prev_binom array  */

    /* First call: initialize and build row 0. */
    if (cache == NULL) {
        cache_capacity = 4;
        cache = alloc_seq_array((size_t)cache_capacity);
        cache_size = 0;
        max_row = 0;
        prev_binom = NULL;
        prev_len = 0;
        build_row(0, &cache, &cache_size, &cache_capacity,
                  &prev_binom, &prev_len);
    }

    /* Determine the row that contains index n and build rows up to it. */
    while ((max_row + 1) * (max_row + 2) / 2 <= n) {
        build_row(max_row + 1, &cache, &cache_size, &cache_capacity,
                  &prev_binom, &prev_len);
        max_row++;
    }

    return cache[n];
}

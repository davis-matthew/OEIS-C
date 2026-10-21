/*
 * A096137  Table read by rows: row n contains the sum of each nonempty subset
 *          of {1, 2, ..., n}. In each row, the sums are arranged in ascending
 *          order.
 * https://oeis.org/A096137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9775, u16 n <= 9775, u32 n <= 9775, u64 n <= 9775,
 *            u128 n <= 9775, big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A096137: the n‑th element in the flattened table whose
   k‑th row consists of the sums of all non‑empty subsets of
   {1,2,…,k} arranged in ascending order. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Headers for sequence array helpers that work in C++ (arbitrary‑precision) */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Static cache that stores the sequence values already generated. */
static seq_t *cache   = NULL;      /* array holding the values */
static size_t cache_len = 0;       /* number of elements filled */
static size_t cache_cap = 0;       /* allocated capacity */
static idx_t  last_row  = 0;       /* last row that has been processed */

/* Ensure that the cache has at least `needed` slots.  If the capacity
   has to be increased, use seq_realloc, which is safe for arbitrary‑
   precision seq_t objects in C++. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp)
        exit(EXIT_FAILURE);         /* fatal allocation error */
    cache    = tmp;
    cache_cap = new_cap;
}

/* Generate rows of the table until the cache contains at least `n`
   elements.  Each row `row` has `2^row-1` sums. */
static void expand_to(idx_t n)
{
    if (n <= cache_len)
        return;                      /* already have enough */

    for (idx_t row = last_row + 1; ; ++row) {
        /* Max possible sum for row `row`: 1+2+...+row = row*(row+1)/2 */
        unsigned int max_sum = (unsigned int)(row * (row + 1) / 2);

        /* frequency array: freq[s] = number of subsets with sum s */
        unsigned int *freq = (unsigned int *)calloc(max_sum + 1, sizeof(unsigned int));
        if (!freq)
            exit(EXIT_FAILURE);
        freq[0] = 1;                  /* empty subset */

        unsigned int cur_max = 0;
        for (idx_t i = 1; i <= row; ++i) {
            cur_max += (unsigned int)i;
            for (int s = cur_max; s >= i; --s) {
                freq[s] += freq[s - (unsigned int)i];
            }
        }

        /* Count how many non‑empty subsets exist for this row. */
        size_t row_cnt = 0;
        for (unsigned int s = 1; s <= max_sum; ++s)
            row_cnt += freq[s];

        /* Append the sums to the cache, already in ascending order. */
        ensure_cache_capacity(cache_len + row_cnt);

        size_t pos = cache_len;
        for (unsigned int s = 1; s <= max_sum; ++s) {
            for (unsigned int k = 0; k < freq[s]; ++k)
                cache[pos++] = (seq_t)s;
        }
        cache_len = pos;

        free(freq);
        last_row = row;

        if (cache_len >= (size_t)n)
            break;                 /* reached required length */
    }
}

/* Return the n-th term (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    expand_to(n);
    return cache[(size_t)n - 1];
}

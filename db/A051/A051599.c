/*
 * A051599  Rows of triangle formed using Pascal's rule except begin and end
 *          n-th row with (n+1)st prime.
 * https://oeis.org/A051599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 158, u32 n <= 574, u64 n <= 2176,
 *            u128 n <= 5150, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051599:  triangle rows built from Pascal's rule, each row
   beginning and ending with the (row+1)-st prime.  a(n) is the n-th
   element of the flattened triangle. */

/* Standard types */
#include <stdint.h>
#include <stdlib.h>

/* Get the k‑th prime (k is 1‑based).  The function returns a seq_t. */
#include "../utils/get_prime.h"

/* Helper to grow a linear cache of seq_t values.  The growth factor is
   2; the cache always has enough capacity for the next row that will be
   added. */
static seq_t *ensure_cache(idx_t needed, seq_t *cache, size_t *cap)
{
    if (needed > *cap) {
        size_t new_cap = *cap ? *cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        *cap = new_cap;
    }
    return cache;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;      /* flat array of all generated values */
    static idx_t cache_size = 0;     /* number of elements stored */
    static idx_t cache_cap = 0;      /* allocated capacity (elements) */
    static idx_t next_row = 0;       /* next row index to generate (0‑based) */

    /* Build rows until we have at least n+1 elements in the cache. */
    while (cache_size <= n) {
        /* Length of the upcoming row. */
        idx_t row_len = next_row + 1;

        /* Expand the cache if necessary before inserting the new row. */
        cache = ensure_cache(cache_size + row_len, cache, &cache_cap);

        /* The prime to use at both ends of this row. */
        seq_t prime = (seq_t)get_prime(next_row + 1);

        /* Starting position of the previous row (0 when next_row==0). */
        idx_t prev_start = cache_size - (next_row);

        /* Generate the new row. */
        for (idx_t j = 0; j < row_len; ++j) {
            if (j == 0 || j == row_len - 1) {
                /* First or last element: the prime. */
                cache[cache_size++] = prime;
            } else {
                /* Interior element: sum of two adjacent entries from
                   the previous row. */
                seq_t left  = cache[prev_start + j - 1];
                seq_t right = cache[prev_start + j];
                cache[cache_size++] = left + right;
            }
        }

        ++next_row; /* move to the next row index */
    }

    /* Return the requested element. */
    return cache[n];
}

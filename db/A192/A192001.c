/*
 * A192001  Triangle with sums of nonnegative integer powers of positive first
 *          n integers in the columns.
 * https://oeis.org/A192001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing OEIS A192001.
   The sequence consists of the entries of the triangle
   a(r,m) = Sum_{k=1..r-m} k^m   (r>=1, 0<=m<r),
   concatenated row by row (row 1 first, then row 2, etc.).
   The function caches computed terms so any order of calls works. */

#include <stdlib.h>   // malloc, realloc
#include <stddef.h>   // size_t
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  // safety; offset starts at 1

    static seq_t *cache = NULL;      // dynamic cache of terms
    static size_t cache_cap = 0;     // allocated capacity in terms
    static size_t cache_size = 0;    // number of terms currently stored
    static idx_t next_row = 1;       // next row to generate

    /* target is zero‑based index (n‑1) */
    size_t target = (size_t)(n - 1);

    /* extend cache until it contains the target term */
    while (cache_size <= target) {
        idx_t r = next_row;                 // current row number (>=1)
        size_t needed = cache_size + (size_t)r;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 128;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t start = cache_size;          // index of first element of this row

        /* column 0: a(r,0) = r */
        cache[start] = (seq_t)r;

        /* remaining columns m = 1 .. r-1 */
        for (idx_t m = 1; m < r; ++m) {
            seq_t sum = 0;
            idx_t max_k = r - m;            /* upper bound for k in the sum */
            for (idx_t k = 1; k <= max_k; ++k) {
                sum += pow_int((seq_t)k, m);
            }
            cache[start + (size_t)m] = sum;
        }

        cache_size += (size_t)r;
        ++next_row;
    }

    return cache[target];
}

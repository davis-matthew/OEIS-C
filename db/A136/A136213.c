/*
 * A136213  Triple factorial triangle, read by rows of 3n(n+1)/2+1 terms,
 *          where row n+1 is generated from row n by first inserting zeros in
 *          row n at positions {[m*(m+5)/6], m=0..3n-1} and then taking
 *          partial sums, starting with a '1' in row 0.
 * https://oeis.org/A136213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the triple factorial triangle
 *            (A136213). * Construction: * Row 0 = [1]. * For r ≥ 0, row r+1
 *            is produced from row r by: * – inserting zeros at positions
 *            ⌊m(m+5)/6⌋ for m = 0 … 3r‑1, and * – taking suffix sums of the
 *            resulting array. * The sequence is read row by row
 * Range:     u8 n <= 33, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of the triangle row values */
    static seq_t *cache      = NULL;   /* contiguous array of terms   */
    static size_t cache_len  = 0;      /* number of stored terms        */
    static size_t cache_cap  = 0;      /* allocated capacity            */
    static size_t prev_row_len = 0;    /* length of the last stored row  */
    static idx_t  row_index  = 0;      /* index of the last stored row  */

    /* Initialise the first row: [1] */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* out of memory */
        cache[0] = (seq_t)1;                   /* a(0) */
        cache_len   = 1;
        prev_row_len = 1;
        row_index   = 0;
    }

    /* Generate rows until the desired index n is covered */
    while (cache_len <= n) {
        idx_t  next_row     = row_index + 1;   /* row to generate          */
        idx_t  zero_count   = 3 * next_row;   /* number of zeros to insert*/
        idx_t  old_len      = prev_row_len;   /* length of previous row   */
        idx_t  new_len      = old_len + zero_count;

        /* Ensure enough space in the cache */
        if (cache_len + new_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (cache_len + new_len > new_cap) new_cap *= 2;
            seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp_cache) {
                free(cache);
                return (seq_t)0;
            }
            cache   = tmp_cache;
            cache_cap = new_cap;
        }

        /* Temporary array for the new row */
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;

        /* Build the new row by inserting zeros and copying old values */
        idx_t  pos_idx       = 0;                /* index of next zero pos   */
        size_t old_start     = cache_len - (size_t)old_len;
        idx_t  old_idx       = 0;                /* index in old row         */

        for (idx_t i = 0; i < new_len; ++i) {
            if (pos_idx < zero_count) {
                idx_t cur_pos = (pos_idx * (pos_idx + 5)) / 6;
                if (i == cur_pos) {
                    tmp[i] = (seq_t)0;
                    ++pos_idx;
                    continue;
                }
            }
            tmp[i] = cache[old_start + (size_t)old_idx];
            ++old_idx;
        }

        /* Compute suffix sums in place */
        seq_t running = 0;
        for (idx_t i = new_len; i-- > 0; ) {
            running += tmp[i];
            tmp[i] = running;
        }

        /* Append the new row into the cache */
        for (idx_t i = 0; i < new_len; ++i) {
            cache[cache_len + i] = tmp[i];
        }

        free(tmp);
        cache_len   += new_len;
        prev_row_len = new_len;
        ++row_index;
    }

    return cache[(size_t)n];
}

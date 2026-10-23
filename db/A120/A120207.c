/*
 * A120207  a(1)=6; a(n)=floor((59+sum(a(1) to a(n-1)))/9).
 * https://oeis.org/A120207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6; a(n)=floor((59+sum_{i=1}^{n-1} a(i))/9) */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for values and their prefix sums (0‑based). */
    static seq_t *seq_cache   = NULL; /* a values, seq_cache[i] = a(i+1)   */
    static seq_t *sum_cache   = NULL; /* prefix sums, sum_cache[i] = ℎ(i+1) */
    static size_t cache_len   = 0;     /* number of stored terms */
    static size_t cache_cap   = 0;     /* allocated capacity          */

    if (n == 0)          /* safety guard – offset is 1                  */
        return 0;

    /* Grow cache until it contains the requested term.           */
    while (cache_len < n)
    {
        /* Allocate or expand storage.                            */
        if (cache_len == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *tmp_seq = (seq_t *)realloc(seq_cache,
                                               new_cap * sizeof(seq_t));
            seq_t *tmp_sum = (seq_t *)realloc(sum_cache,
                                               new_cap * sizeof(seq_t));
            if (!tmp_seq || !tmp_sum)          /* allocation failed */
            {
                /* If allocation failed, keep old data and return 0. */
                return 0;
            }
            seq_cache = tmp_seq;
            sum_cache = tmp_sum;
            cache_cap = new_cap;
        }

        /* Compute the next term.                                   */
        if (cache_len == 0)
        {
            /* a(1) = 6 */
            seq_cache[0] = 6;
            sum_cache[0] = 6;
        }
        else
        {
            seq_t prev_sum = sum_cache[cache_len - 1];    /* Σ_{i=1}^{n-1} a(i) */
            seq_t val = ((seq_t)59 + prev_sum) / 9;      /* floor division    */
            seq_cache[cache_len] = val;
            sum_cache[cache_len] = prev_sum + val;
        }
        cache_len++;
    }

    return seq_cache[n - 1];
}

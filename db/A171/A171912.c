/*
 * A171912  Van Eck sequence (cf. A181391) starting with 2.
 * https://oeis.org/A171912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Van Eck sequence starting with 2, defined
 *            recursively by a(n+1) = min{k>0 | a(n-k)=a(n)} or 0 if no such k
 *            exists. Here a(1)=2
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;
    static idx_t seq_cap = 0, seq_len = 0;
    static idx_t *last_occ = NULL;
    static idx_t last_cap = 0;

    /* initialise on first call */
    if (seq_len == 0)
    {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)2;            /* a(1) = 2 */
        seq_len = 1;

        last_cap = 16;
        last_occ = (idx_t *)calloc(last_cap, sizeof(idx_t));
    }

    /* if we already have the term, return it immediately */
    if (n <= seq_len)
        return seq[n - 1];

    /* extend seq array if necessary */
    if (n > seq_cap)
    {
        idx_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* generate missing terms up to n */
    for (idx_t i = seq_len; i < n; ++i)
    {
        /* value at position i (1‑based) is seq[i-1] */
        seq_t prev_val = seq[i - 1];
        idx_t v_idx = (idx_t)prev_val;

        /* ensure last_occ has room for v_idx */
        if (v_idx >= last_cap)
        {
            idx_t new_cap = last_cap ? last_cap * 2 : 16;
            while (new_cap <= v_idx)
                new_cap *= 2;
            last_occ = (idx_t *)realloc(last_occ, new_cap * sizeof(idx_t));
            /* initialise new slots to 0 */
            for (idx_t j = last_cap; j < new_cap; ++j)
                last_occ[j] = 0;
            last_cap = new_cap;
        }

        idx_t prev_index = last_occ[v_idx];
        seq_t next_val = (prev_index == 0) ? (seq_t)0
                                            : (seq_t)(i - prev_index);

        seq[i] = next_val;

        /* update last occurrence of prev_val to current index i (1‑based) */
        last_occ[v_idx] = i;
    }

    seq_len = n;
    return seq[n - 1];
}

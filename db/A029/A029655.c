/*
 * A029655  Numbers in the (2,1)-Pascal triangle A029653 that are different
 *          from 2.
 * https://oeis.org/A029655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Triangle T defined by
   T(0,0)=1, T(1,0)=1,
   T(n,0)=T(n-1,0)+2   (n>=2),
   T(n,k)=T(n-1,k-1)+T(n-1,k)  (1≤k≤n).
   The sequence a(n) is the flattened row‑wise enumeration of T,
   skipping any element equal to 2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;        /* number of terms stored in seq   */

    static seq_t *prev = NULL;
    static size_t prev_len = 0;       /* length of previous row           */

    /* initialise on first call */
    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));

        prev_len = 1;
        prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        prev[0] = (seq_t)1;

        seq_len = 0;
        seq[seq_len++] = (seq_t)1;   /* a(0) */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)1;   /* a(1) – row 1 equals row 0 */
    }

    /* generate rows until we have at least n+1 terms stored */
    while (seq_len <= (size_t)n) {
        size_t new_len = prev_len + 1;
        seq_t *new_row = (seq_t *)malloc(new_len * sizeof(seq_t));

        new_row[0] = prev[0] + (seq_t)2;
        for (size_t i = 1; i + 0 < new_len - 1; ++i)
            new_row[i] = prev[i - 1] + prev[i];
        if (new_len > 0)
            new_row[new_len - 1] = prev[prev_len - 1];

        for (size_t i = 0; i < new_len; ++i) {
            seq_t v = new_row[i];
            if (v != (seq_t)2) {
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = v;
            }
        }

        free(prev);
        prev = new_row;
        prev_len = new_len;
    }

    return seq[(size_t)n];
}

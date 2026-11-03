/*
 * A078990  Triangle arising from (4,2) tennis ball problem, read by rows.
 * https://oeis.org/A078990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 44, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(0,0)=1.
   For r>=1:
     T(r,0)=1.
     Let d_0 = T(r-1,0).
     For k=1..2r:
         d_k = T(r-1,k)+d_{k‑1}   (treat T(r-1,k)=0 if k exceeds the previous row length)
         T(r,k) = T(r,k-1)+d_k.
   a(n) is the flattened sequence obtained by reading the triangle rows in order. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *seq      = NULL;
    static size_t  seq_len = 0, seq_cap = 0;
    static seq_t *prev     = NULL;
    static size_t  prev_len = 0;
    static size_t  row      = 0;   /* last generated row index (0‑based) */

    /* initialise on first call */
    if (seq_len == 0) {
        prev_len = 1;
        prev = mem_new(seq_t, 1);
        prev[0] = (seq_t)1;

        seq_cap = 256;
        seq = mem_new(seq_t, seq_cap);
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* extend the triangle until we have at least n+1 terms */
    while (seq_len <= n) {
        size_t r      = row + 1;          /* new row index */
        size_t new_len = 2 * r + 1;       /* length of new row   */
        seq_t *next = mem_new(seq_t, new_len);

        /* build the new row */
        next[0] = (seq_t)1;
        seq_t d = prev[0];
        for (size_t k = 1; k < new_len; ++k) {
            seq_t val_prev = (k < prev_len) ? prev[k] : (seq_t)0;
            d = d + val_prev;
            next[k] = next[k-1] + d;
        }

        /* append the new row to the flattened sequence */
        if (seq_len + new_len > seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 256;
            while (seq_len + new_len > seq_cap) seq_cap *= 2;
            seq = mem_resize(seq_t, seq, seq_cap);
        }
        for (size_t i = 0; i < new_len; ++i)
            seq[seq_len + i] = next[i];
        seq_len += new_len;

        /* prepare for the next iteration */
        mem_free(prev);
        prev     = next;
        prev_len = new_len;
        row      = r;
    }

    return seq[n];
}

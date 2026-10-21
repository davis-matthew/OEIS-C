/*
 * A052739  E.g.f. (1-sqrt(1-4x-4x^2))/2 -x*(1+x).
 * https://oeis.org/A052739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 0, a(1) = 0, a(2) = 2, a(3) = 24, a(4) = 288.
   For n ≥ 3:  a(n+2) = (4*n^2‑4)·a(n) + (4*n+2)·a(n+1).   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the sequence (indices 0…size‑1)    */
    static seq_t *seq = NULL;
    static idx_t seq_size = 0;

    if (seq_size <= n) {
        /* required size (at least 5 to store the initial terms) */
        idx_t need = (n + 1 > 5) ? n + 1 : 5;

        /* allocate or enlarge the storage */
        seq = (seq_t *)realloc(seq, need * sizeof(seq_t));

        /* initialise the first five values when the array is first created */
        if (seq_size == 0) {
            seq[0] = (seq_t)0;
            seq[1] = (seq_t)0;
            seq[2] = (seq_t)2;
            seq[3] = (seq_t)24;
            seq[4] = (seq_t)288;
        }

        /* compute the missing terms */
        idx_t start = (seq_size > 5) ? seq_size : 5;
        for (idx_t i = start; i <= n; ++i) {
            seq_t ii      = (seq_t)(i - 2);                      /* i‑2 as seq_t      */
            seq_t coeff1  = (seq_t)4U * ii * ii - (seq_t)4U;      /* 4*(i‑2)^2‑4       */
            seq_t coeff2  = (seq_t)4U * ii + (seq_t)2U;          /* 4*(i‑2)+2         */
            seq[i] = seq[i - 2] * coeff1 + seq[i - 1] * coeff2;  /* recurrence step   */
        }

        seq_size = need; /* update cache size                */
    }

    return seq[n];
}

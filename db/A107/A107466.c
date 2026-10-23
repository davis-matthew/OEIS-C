/*
 * A107466  Numbers of the form (5^i)*(13^j).
 * https://oeis.org/A107466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest of {5^i * 13^j : i,j ≥ 0}
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined offset, return 0 as a placeholder */

    static seq_t *seq = NULL;   /* array of computed terms, 0‑indexed: seq[0] = a(1) */
    static idx_t len    = 0;    /* number of terms stored in seq          */
    static idx_t i5     = 0;    /* index for next multiple of 5           */
    static idx_t i13    = 0;    /* index for next multiple of 13          */

    /* Initialise the sequence with the first term */
    if (seq == NULL) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        seq[0] = (seq_t)1;      /* a(1) = 1 */
        len   = 1;
    }

    /* Expand the array until we have at least n terms */
    while (len < n) {
        seq_t next5  = seq[i5] * (seq_t)5;
        seq_t next13 = seq[i13] * (seq_t)13;
        seq_t next   = next5 < next13 ? next5 : next13;

        /* Advance the pointers that produced the chosen next value */
        if (next5 <= next13) i5++;
        if (next13 <= next5) i13++;

        /* Grow the array to hold the new term */
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (len + 1));
        seq[len] = next;
        len++;
    }

    return seq[n - 1];
}

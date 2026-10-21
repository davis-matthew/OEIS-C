/*
 * A079709  Numbers m such that the squarefree kernel of m is less than the
 *          squarefree kernel of m+1.
 * https://oeis.org/A079709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079709: a(n) = the n‑th integer m with rad(m) < rad(m+1)
 * rad(x) is the squarefree kernel of x (product of distinct prime
 * factors).  Since there is no closed formula, the function searches
 * for terms incrementally and memoises them.  It works for any build
 * where seq_t and idx_t are unsigned integer types and for any order
 * of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rad.h"   /* rad(idx_t) -> seq_t */

seq_t a(idx_t n)
{
    /* Dynamic memoisation array: seq[0] == a(1), seq[1] == a(2), ... */
    static seq_t  *seq   = NULL;      /* pointer to array of terms     */
    static size_t  seq_len = 0;       /* number of terms stored          */
    static size_t  seq_cap = 0;       /* current array capacity          */
    static idx_t   next_m  = 1;       /* next integer to test            */

    /* Allocate initial capacity on first call. */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        seq_t rad_m  = rad(next_m);       /* rad(m)      */
        seq_t rad_m1 = rad(next_m + 1);   /* rad(m+1)    */

        if (rad_m < rad_m1) {             /* condition satisfied */
            if (seq_len >= seq_cap) {    /* double capacity if needed */
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)next_m;   /* store m as a(n) */
        }
        ++next_m;                          /* move to next integer */
    }

    return seq[n - 1];   /* indices in array are 0‑based */
}

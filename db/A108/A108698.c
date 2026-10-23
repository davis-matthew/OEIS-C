/*
 * A108698  Numbers of the form (6^i)*(11^j), with i, j >= 0.
 * https://oeis.org/A108698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 70, u64 n <= 253, u128 n <= 962,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: sorted list of positive integers of the form 6^i * 11^j,
   with i, j ≥ 0.  The function returns the nth term (a(1) = 1). */
#include <stdlib.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t size   = 0;   /* number of elements stored          */
    static idx_t cap    = 0;   /* current allocated capacity         */
    static idx_t i_ptr  = 0;   /* index for the 6‑multiples          */
    static idx_t j_ptr  = 0;   /* index for the 11‑multiples         */

    /* Initialise on first call. */
    if (!arr) {
        cap  = 16;
        arr  = seq_malloc(cap);
        if (!arr) exit(1);      /* out of memory */
        arr[0] = (seq_t)1;      /* a(1) = 1 */
        size  = 1;
        i_ptr = 0;
        j_ptr = 0;
    }

    /* Generate terms until the requested index is reached. */
    while (size < n) {
        seq_t next1 = arr[i_ptr] * (seq_t)6;   /* 6 * a(i_ptr)   */
        seq_t next2 = arr[j_ptr] * (seq_t)11;  /* 11 * a(j_ptr)  */
        seq_t next  = (next1 < next2) ? next1 : next2;

        /* Advance pointers corresponding to the produced value. */
        if (next == next1) ++i_ptr;
        if (next == next2) ++j_ptr;

        /* Append only if distinct from the last stored term. */
        if (next != arr[size - 1]) {
            if (size == cap) {          /* grow the array */
                cap = cap * 2;
                seq_t *tmp = seq_realloc(arr, cap);
                if (!tmp) exit(1);
                arr = tmp;
            }
            arr[size++] = next;
        }
    }

    return arr[n - 1];
}

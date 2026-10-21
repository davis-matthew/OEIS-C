/*
 * A069353  Numbers of form 2^i*3^j - 1 with i, j >= 0.
 * https://oeis.org/A069353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^i * 3^j - 1 , i,j >= 0 (sorted ascending)
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds the sequence of all 2‑3 smooth numbers (products
   of powers of 2 and 3) in increasing order and returns the (n‑1)th
   value minus one.  It uses a two‑pointer merge algorithm that runs
   in O(n) time and constant extra space (apart from the sequence
   array).  Memoization is employed so repeated calls are efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the generated smooth numbers */
    static seq_t *smooth = NULL;
    static idx_t size   = 0;   /* number of terms currently stored */
    static idx_t cap    = 0;   /* current capacity of the array */
    static idx_t p2 = 0;       /* index for multiplying by 2 */
    static idx_t p3 = 0;       /* index for multiplying by 3 */

    /* generate terms until we have at least n of them */
    while (size < n) {
        /* initial allocation when first called */
        if (size == 0) {
            if (cap == 0) {
                cap = 16;
                smooth = (seq_t *)malloc(cap * sizeof(seq_t));
            }
            smooth[0] = (seq_t)1;   /* 2^0 * 3^0 = 1 */
            size = 1;
            continue;               /* loop again to start generating */
        }

        /* grow the array if needed */
        if (size >= cap) {
            cap = cap ? cap * 2 : 16;
            smooth = (seq_t *)realloc(smooth, cap * sizeof(seq_t));
        }

        /* next candidates from the two pointers */
        seq_t next2 = smooth[p2] * (seq_t)2;
        seq_t next3 = smooth[p3] * (seq_t)3;
        seq_t next  = next2 < next3 ? next2 : next3;

        smooth[size] = next;

        /* advance pointers that produced the chosen next value */
        if (next == next2) p2++;
        if (next == next3) p3++;

        size++;
    }

    /* return the desired term minus one */
    return smooth[n - 1] - (seq_t)1;
}

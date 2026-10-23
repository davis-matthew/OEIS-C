/*
 * A041039  Denominators of continued fraction convergents to sqrt(24).
 * https://oeis.org/A041039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A041039 – denominators of continued fraction convergents to sqrt(24).
   Recurrence (used on an auxiliary sequence b):
     b[0] = 0,  b[1] = 1
     for k ≥ 2:
        if k is even:   b[k] = b[k-1] + b[k-2]
        if k is odd:    b[k] = 8*b[k-1] + b[k-2]
   The requested term a(n) is simply a(n) = b[n+1] (so that a(0)=1, a(1)=1, …).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *b   = NULL;   /* auxiliary sequence b */
    static idx_t  size = 0;     /* current allocated size of b */

    /* Ensure that we have indices 0 .. n+1 available in b */
    if (size < n + 2) {
        idx_t newsize = n + 2;
        if (b == NULL) {
            b = (seq_t *)malloc(newsize * sizeof(seq_t));
        } else {
            b = (seq_t *)realloc(b, newsize * sizeof(seq_t));
        }
        /* Initialize new entries */
        if (size == 0) {          /* first allocation */
            b[0] = (seq_t)0;
            b[1] = (seq_t)1;
            size = 2;
        }
        /* Compute missing terms */
        for (idx_t k = size; k < newsize; ++k) {
            if (k & 1U) {                             /* k odd */
                b[k] = (seq_t)8 * b[k-1] + b[k-2];
            } else {                                  /* k even */
                b[k] = b[k-1] + b[k-2];
            }
        }
        size = newsize;
    }

    return b[n + 1];   /* a(n) = b[n+1] */
}

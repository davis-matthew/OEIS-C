/*
 * A087299  Ratio of volume of n-dimensional ball to circumscribing n-cube is
 *          Pi^floor(n/2) divided by a(n).
 * https://oeis.org/A087299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n)=2*n*a(n-2)  for n>=2 */
/* This implementation computes the value directly in O(n) time using only two temporary variables. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    seq_t prev_even = (seq_t)1; /* a(0) */
    seq_t prev_odd  = (seq_t)1; /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even index */
            cur = prev_even * (((seq_t)2) * ((seq_t)i));
            prev_even = cur;
        } else {                          /* odd index */
            cur = prev_odd * (((seq_t)2) * ((seq_t)i));
            prev_odd = cur;
        }
    }
    return cur;
}

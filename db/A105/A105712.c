/*
 * A105712  Smallest m such that 2 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   pow10_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105712: smallest m such that the decimal digit at
   position n (0‑based from the least significant digit) of the
   m‑th Fibonacci number Fm is 2. */
#include <stdlib.h>
#include "../utils/pow10_idx.h"

/* Return a(n) for any n >= 0.  The function builds Fibonacci numbers
   on the fly and stops at the first occurrence of digit 2 at the
   requested position. */
seq_t a(idx_t n)
{
    /* 10^n is used to isolate the desired digit. */
    seq_t ten_pow = pow10_idx(n);

    /* Fibonacci base cases. */
    seq_t fp_prevprev = 0;   /* F0 */
    seq_t fp_prev     = 1;   /* F1 */

    /* Test F0 and F1 explicitly. */
    if ((fp_prevprev / ten_pow) % 10 == 2)
        return fp_prevprev;
    if ((fp_prev / ten_pow) % 10 == 2)
        return fp_prev;

    /* Iterate for m >= 2: Fm = F(m-1) + F(m-2). */
    for (idx_t m = 2;; ++m) {
        seq_t fp_cur = fp_prev + fp_prevprev;   /* Fm */

        if ((fp_cur / ten_pow) % 10 == 2)
            return fp_cur;                     /* found minimal m */

        /* Prepare for next iteration. */
        fp_prevprev = fp_prev;
        fp_prev     = fp_cur;
    }
}

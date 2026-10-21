/*
 * A075483  Number of iteration that first becomes smaller than the initial
 *          value if Collatz-function (A006370) is iterated, starting with
 *          numbers of form 64n+63.
 * https://oeis.org/A075483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the number of terms until the first value becomes smaller
   than the initial value for the Collatz sequence starting with
   64 n + 63.  The sequence a(n) is defined as the position m (1‑based)
   such that the m‑th term of the sequence is the first one less than
   64 n + 63.  This is a straightforward simulation of the Collatz
   iteration. */

#include <stdint.h>   /* for the integer types used in the build */

seq_t a(idx_t n)
{
    /* starting value 64 n + 63 */
    seq_t start = (seq_t)n * (seq_t)64 + (seq_t)63;
    seq_t cur   = start;
    seq_t step  = 1;               /* the starting value itself is term 1 */

    for (;;)
    {
        /* apply Collatz function */
        cur = (cur & 1) ? (cur * (seq_t)3 + (seq_t)1) : (cur >> 1);
        ++step;                    /* now pointing to the current term */

        if (cur < start)
            return step;           /* this is the position of the first
                                      sub‑merging term */
    }
}

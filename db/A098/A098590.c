/*
 * A098590  a(n) = 4^n for n = 0..3; for n > 3, a(n) = 4*a(n-1) + a(n-4).
 * https://oeis.org/A098590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098590
   a(0) = 1, a(1) = 4, a(2) = 16, a(3) = 64
   a(n) = 4*a(n-1) + a(n-4)   for n > 3
   The implementation evaluates the recurrence iteratively without any
   dynamic allocation.                                                    */
#include <stdlib.h>   /* for idx_t */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)64;

    /* Sliding window of the last four computed values. */
    seq_t t0 = (seq_t)1;   /* a(0) */
    seq_t t1 = (seq_t)4;   /* a(1) */
    seq_t t2 = (seq_t)16;  /* a(2) */
    seq_t t3 = (seq_t)64;  /* a(3) */
    seq_t t4;              /* temporary for a(i) */

    /* Compute a(4) … a(n) iteratively. */
    for (idx_t i = 4; i <= n; ++i) {
        t4 = t3 * (seq_t)4 + t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
    }

    return t3;
}

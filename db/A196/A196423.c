/*
 * A196423  Number of n X 1 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors less than itself.
 * https://oeis.org/A196423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4)
   with initial values a(1)=1, a(2)=3, a(3)=5, a(4)=11. */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Handle the offset: only n >= 1 is defined. */
    if (n == 0) {
        return (seq_t)0;   /* unused, but safe */
    }

    /* Base cases. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;

    /* Iterative computation using only the last four values. */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)3;   /* a(2) */
    seq_t a3 = (seq_t)5;   /* a(3) */
    seq_t a4 = (seq_t)11;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a4 + a3 + (seq_t)3 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}

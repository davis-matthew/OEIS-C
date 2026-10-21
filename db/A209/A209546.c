/*
 * A209546  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=20, a(3)=61, a(4)=191; For n≥5: a(n) = 5*a(n-1) -
 *            6*a(n-2) + 2*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the first four known values directly. */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)61;
    if (n == 4) return (seq_t)191;

    /* Iterate from n=5 up to the requested n using a sliding window
       of the last four terms.  This works for all integer types
       that support the usual arithmetic operators. */
    seq_t a_tm4 = (seq_t)7;   /* a(n-4) */
    seq_t a_tm3 = (seq_t)20;  /* a(n-3) */
    seq_t a_tm2 = (seq_t)61;  /* a(n-2) */
    seq_t a_tm1 = (seq_t)191; /* a(n-1) */
    seq_t current;

    for (idx_t i = 5; i <= n; ++i) {
        current = 5 * a_tm1 - 6 * a_tm2 + 2 * a_tm4;
        a_tm4 = a_tm3;
        a_tm3 = a_tm2;
        a_tm2 = a_tm1;
        a_tm1 = current;
    }
    return current;
}

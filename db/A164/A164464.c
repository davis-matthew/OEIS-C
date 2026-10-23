/*
 * A164464  Number of binary strings of length n with no substrings equal to
 *          0001, 0100, or 0111.
 * https://oeis.org/A164464
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + 3 for n > 6 Initial values: a(4) = 13,
 *            a(5) = 20, a(6) = 31
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        return (seq_t)0;          /* undefined for n < 4, return 0 */
    }
    if (n == 4) {
        return (seq_t)13;
    }
    if (n == 5) {
        return (seq_t)20;
    }
    if (n == 6) {
        return (seq_t)31;
    }

    /* Compute iteratively using the recurrence.
       a3 = a(n-3), a4 = a(n-2), a5 = a(n-1) for the current n */
    seq_t a3 = (seq_t)13;    /* a(4) */
    seq_t a4 = (seq_t)20;    /* a(5) */
    seq_t a5 = (seq_t)31;    /* a(6) */
    seq_t cur = 0;

    for (idx_t i = 7; i <= n; ++i) {
        cur = a5 + a3 + (seq_t)3;   /* a(i) = a(i-1) + a(i-3) + 3 */

        /* shift to next step */
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }
    return cur;
}

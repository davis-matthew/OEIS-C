/*
 * A229841  Number of n X 2 0..2 arrays x(i,j) with each element horizontally
 *          or vertically next to at least one element with value 2-x(i,j).
 * https://oeis.org/A229841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) - a(n-3) - 5*a(n-4), with a(1)=3,
 *            a(2)=15, a(3)=81, a(4)=423
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly. */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)81;
    if (n == 4) return (seq_t)423;

    /* Variables for the four most recent terms. */
    seq_t a1 = (seq_t)3;   /* a(n-4) */
    seq_t a2 = (seq_t)15;  /* a(n-3) */
    seq_t a3 = (seq_t)81;  /* a(n-2) */
    seq_t a4 = (seq_t)423; /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)5 * a4 + (seq_t)2 * a3 - a2 - (seq_t)5 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = cur;
    }
    return a4;
}

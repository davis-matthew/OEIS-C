/*
 * A033127  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-3) - 9*a(n-4) ; Base values: a(1)=1,
 *            a(2)=9, a(3)=82, a(4)=739
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)82;
    if (n == 4) return (seq_t)739;

    seq_t nm4 = (seq_t)1;   /* a(n-4) */
    seq_t nm3 = (seq_t)9;   /* a(n-3) */
    seq_t nm2 = (seq_t)82;  /* a(n-2) */
    seq_t nm1 = (seq_t)739; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = 9 * nm1 + nm3 - 9 * nm4;
        nm4 = nm3;
        nm3 = nm2;
        nm2 = nm1;
        nm1 = val;
    }
    return nm1;
}

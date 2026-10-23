/*
 * A136786  Number of primitive multiplex juggling sequences of length n, base
 *          state <2,1> and hand capacity 3.
 * https://oeis.org/A136786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 22*a(n-2) + 13*a(n-3) + 3*a(n-4) for n>4, with
 *            a(1)=1, a(2)=4, a(3)=21, a(4)=111
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) corresponds to n==1. */
    if (n == 0)
        return 0;   /* behavior for n < 1 is irrelevant */

    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 21;
    if (n == 4) return 111;

    /* Keep only the last four values, thus no dynamic memory is needed. */
    seq_t a1 = 1;   /* a(n-4) */
    seq_t a2 = 4;   /* a(n-3) */
    seq_t a3 = 21;  /* a(n-2) */
    seq_t a4 = 111; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 9 * a4 - 22 * a3 + 13 * a2 + 3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

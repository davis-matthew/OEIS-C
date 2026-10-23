/*
 * A027286  a(n) = Sum_{k=0..2n} (k+1) * A026584(n, k).
 * https://oeis.org/A027286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2)-8*a(n-3)-16*a(n-4) with initial values
 *            a(0)=1, a(1)=4, a(2)=18, a(3)=56
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)56;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)18;
    seq_t a3 = (seq_t)56;
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)2 * a3 + (seq_t)7 * a2 - (seq_t)8 * a1 - (seq_t)16 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

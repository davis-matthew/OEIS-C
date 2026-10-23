/*
 * A022310  a(n) = a(n-1) + a(n-2) + 1 for n>1, a(0)=0, a(1)=5.
 * https://oeis.org/A022310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0)=0, a(1)=5
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)5;
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 + a0 + (seq_t)1;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}

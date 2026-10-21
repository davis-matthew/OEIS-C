/*
 * A203135  Indices of hexagonal numbers that are also decagonal.
 * https://oeis.org/A203135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) with a(1) = 1, a(2) = 28,
 *            a(3) = 943
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)28;
    if (n == 3) return (seq_t)943;

    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)28;  /* a(n-2) */
    seq_t a3 = (seq_t)943; /* a(n-1) */
    seq_t ai = 0;

    for (idx_t i = 4; i <= n; ++i) {
        ai = (seq_t)35 * a3 - (seq_t)35 * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }

    return ai;
}

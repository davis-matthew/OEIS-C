/*
 * A131515  a(n+2) = 34*a(n+1) - a(n) + 2; a(1) = 1 and a(2) = 16.
 * https://oeis.org/A131515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 16, a(n+2) = 34*a(n+1) - a(n) + 2 for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    seq_t a_nm2 = (seq_t)1; /* a(1) */
    seq_t a_nm1 = (seq_t)16; /* a(2) */
    seq_t a_n;
    for (idx_t i = 3; i <= n; ++i) {
        a_n = (34 * a_nm1) - a_nm2 + (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

/*
 * A090018  a(n) = 6*a(n-1) + 3*a(n-2) for n > 2, a(0)=1, a(1)=6.
 * https://oeis.org/A090018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t, if needed */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)6;   /* a(n-1) */
    seq_t an = 0;          /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)6 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = an;
    }

    return an;
}

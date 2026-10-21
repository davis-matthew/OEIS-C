/*
 * A055790  a(n) = n*a(n-1) + (n-2)*a(n-2), a(0) = 0, a(1) = 2.
 * https://oeis.org/A055790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (n-2)*a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 2;

    seq_t a0 = 0;   /* a(n-2) */
    seq_t a1 = 2;   /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)i * a1 + (seq_t)(i - 2) * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

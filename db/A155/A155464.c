/*
 * A155464  a(n) = 7*a(n-1) - 7*a(n-2) + a(n-3) for n > 2; a(0) = 0, a(1) =
 *          51, a(2) = 340.
 * https://oeis.org/A155464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 34 for n > 1; a(0) = 0, a(1) = 51,
 *            a(2) = 340
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)51;
    if (n == 2) return (seq_t)340;

    seq_t a0 = (seq_t)0;      /* a(0) */
    seq_t a1 = (seq_t)51;     /* a(1) */
    seq_t a2 = (seq_t)340;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = ((seq_t)6 * a2 - a1 + (seq_t)34);
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}

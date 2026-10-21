/*
 * A074522  a(n) = 1^n + 6^n + 9^n.
 * https://oeis.org/A074522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 15*a(n-1) - 54*a(n-2) + 40, a(0)=3, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 1^n + 6^n + 9^n via the linear recurrence. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 3;
    if (n == 1)
        return 16;

    seq_t a0 = 3;   /* a(n-2) */
    seq_t a1 = 16;  /* a(n-1) */
    seq_t a2;       /* a(n) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(n) = 15*a(n-1) - 54*a(n-2) + 40 */
        a2 = 15 * a1 - 54 * a0 + 40;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

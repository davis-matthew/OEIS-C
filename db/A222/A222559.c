/*
 * A222559  a(0) = 0. If n is odd, a(n) = a(n-1) * n, otherwise a(n) = a(n-1)
 *          + n.
 * https://oeis.org/A222559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 32, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n > 0,
   if n is odd:  a(n) = a(n-1) * n
   else        :  a(n) = a(n-1) + n  */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 0;                 /* a(0) */
    if (n == 0)
        return val;

    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1)                 /* n is odd */
            val = val * (seq_t)i;   /* multiplication */
        else
            val = val + (seq_t)i;   /* addition */
    }
    return val;
}

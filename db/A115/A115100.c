/*
 * A115100  Mayan calendar periods in days.
 * https://oeis.org/A115100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(1) = 1 and a(n+1) = a(n) * p[(n-1) % 5] where p = {20, 18, 20,
 *            20, 13}. This gives the Mayan calendar periods in days: 1, 20,
 *            360, 7200, 144000, 1872000,
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* behaviour below offset, not needed */

    static const seq_t p[5] = {
        (seq_t)20,
        (seq_t)18,
        (seq_t)20,
        (seq_t)20,
        (seq_t)13
    };

    seq_t result = (seq_t)1;      /* a(1) */
    if (n == 1)
        return result;

    for (idx_t i = 1; i < n; ++i)
    {
        seq_t factor = p[(i - 1) % 5];
        result = result * factor;
    }
    return result;
}

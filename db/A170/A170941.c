/*
 * A170941  a(n+1) = a(n) + n*a(n-1) - a(n-2) + a(n-3).
 * https://oeis.org/A170941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n) + n*a(n-1) - a(n-2) + a(n-3), a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* sliding window for the last four computed values */
    seq_t a0 = (seq_t)1; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)1; /* a(n-1) */
    seq_t a3 = (seq_t)2; /* a(n)   */

    /* iterate from k = 4 up to n */
    for (idx_t k = 4; k <= n; ++k)
    {
        /* recurrence: a_k = a_{k-1} + (k-1)*a_{k-2} - a_{k-3} + a_{k-4} */
        seq_t ak = a3 + (seq_t)(k - 1) * a2 - a1 + a0;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ak;
    }

    return a3;
}

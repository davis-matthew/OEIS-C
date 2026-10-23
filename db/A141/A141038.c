/*
 * A141038  Padovan-like sequence; a(0)=2, a(1)=1, a(2)=1, a(n) = a(n-2) +
 *          a(n-3).
 * https://oeis.org/A141038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3), with a(0)=2, a(1)=1, a(2)=1
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 79, u64 n <= 158,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;

    seq_t a0 = (seq_t)2; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)1; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t nxt = a1 + a0; /* a(i) = a(i-2) + a(i-3) */
        a0 = a1;             /* shift for next iteration */
        a1 = a2;
        a2 = nxt;
    }
    return a2;
}

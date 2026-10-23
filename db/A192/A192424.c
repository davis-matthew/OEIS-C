/*
 * A192424  a(n) = A192423(n)/2.
 * https://oeis.org/A192424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - a(n-3) - a(n-4), a(0)=1, a(1)=0,
 *            a(2)=2, a(3)=1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)1;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t temp  = a3 + (seq_t)4 * a2;
        seq_t next  = temp - a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

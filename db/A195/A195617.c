/*
 * A195617  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 3.
 * https://oeis.org/A195617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 37*a(n-1) + 37*a(n-2) - a(n-3), with a(1)=35, a(2)=1333,
 *            a(3)=50615
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)35;
    if (n == (idx_t)2) return (seq_t)1333;
    if (n == (idx_t)3) return (seq_t)50615;

    seq_t v0 = (seq_t)35;   /* a(n-3) */
    seq_t v1 = (seq_t)1333; /* a(n-2) */
    seq_t v2 = (seq_t)50615;/* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)37 * v2 + (seq_t)37 * v1 - v0;
        v0 = v1;
        v1 = v2;
        v2 = next;
    }
    return v2;
}

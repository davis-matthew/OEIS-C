/*
 * A169963  Number of (2n+1)-digit squares in carryless arithmetic mod 10.
 * https://oeis.org/A169963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169963: a(n) = 12*a(n-1) - 20*a(n-2),  a(0)=5, a(1)=46 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)46;
    seq_t a0 = (seq_t)5, a1 = (seq_t)46;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = 12 * a1 - 20 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}

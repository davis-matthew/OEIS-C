/*
 * A182536  a(0)=0, a(1)=1, a(n)=(a(n-1) XOR a(n-2)) + n.
 * https://oeis.org/A182536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = (a(n-1) XOR a(n-2)) + n (for n ≥ 2)
 * Range:     u8 n <= 33, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev   = (seq_t)0;      /* a(0) */
    seq_t cur    = (seq_t)1;      /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = (cur ^ prev) + (seq_t)i; /* a(i) = (a(i-1) XOR a(i-2)) + i */
        prev = cur;
        cur  = next;
    }
    return cur;
}

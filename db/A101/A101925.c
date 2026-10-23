/*
 * A101925  a(n) = A005187(n) + 1.
 * https://oeis.org/A101925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005187(n) + 1 Recurrence: a(0)=1; a(2n) = 2*a(n) +
 *            popcount(n) - 1; a(2n+1) = a(2n) + 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n & 1) {                /* n is odd */
        return a(n - 1) + (seq_t)1;
    }
    /* n is even */
    seq_t half = a(n >> 1);                 /* a(n/2)   */
    seq_t pc   = (seq_t)__builtin_popcountll(n >> 1); /* popcount(n/2) */
    return (half << 1) + pc - (seq_t)1;
}

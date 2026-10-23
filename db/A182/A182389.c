/*
 * A182389  a(0)=1, a(n) = (a(n-1) + n) XOR n.
 * https://oeis.org/A182389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1, a(n) = (a(n-1)+n) XOR n
 * Range:     u8 n <= 19, u16 n <= 390, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t cur = 1;          /* a(0) */
    if (n == 0) return cur;
    for (idx_t i = 1; i <= n; ++i) {
        cur = (cur + (seq_t)i) ^ (seq_t)i;
    }
    return cur;
}

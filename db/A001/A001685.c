/*
 * A001685  a(0) = 1, a(1) = 2, a(2) = 3; for n >= 3, a(n) = a(n-2) +
 *          a(n-1)*Product_{i=1..n-3} a(i).
 * https://oeis.org/A001685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-1)*Product_{i=1..n-3} a(i), a(0) = 1, a(1) =
 *            2, a(2) = 3
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    seq_t y = 2;   /* a(i-2) */
    seq_t z = 3;   /* a(i-1) */
    seq_t p = 1;   /* Product_{j=1..i-3} a(j) */
    seq_t cur = 0;
    for (idx_t i = 3; i <= n; i++) {
        cur = y + z * p;
        p *= y;
        y = z;
        z = cur;
    }
    return cur;
}

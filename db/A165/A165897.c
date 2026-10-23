/*
 * A165897  a(n) = a(n-1) + largest proper divisor of a(n-1), a(1)=4.
 * https://oeis.org/A165897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) for n>3, with a(1)=4, a(2)=6, a(3)=9 (equivalent
 *            to a(n)=a(n-1)+largest proper divisor of a(n-1))
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 57, u64 n <= 118,
 *            u128 n <= 239, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)9;

    seq_t a1 = (seq_t)4;   /* a(1) */
    seq_t a2 = (seq_t)6;   /* a(2) */
    seq_t a3 = (seq_t)9;   /* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a1 * (seq_t)3;   /* a(i) = 3 * a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

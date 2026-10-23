/*
 * A198969  a(n) = (11*9^n-1)/2.
 * https://oeis.org/A198969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*9^n-1)/2 Recurrence: a(0)=5, a(n)=9*a(n-1)+4 for n>0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t cur = 5;                     /* a(0) */
    if (n == 0) return cur;

    /* Compute using the recurrence a(n) = 9*a(n-1)+4 */
    for (idx_t i = 1; i <= n; ++i) {
        cur = cur * 9 + 4;
    }
    return cur;
}

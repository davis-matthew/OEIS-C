/*
 * A163607  a(n) = ((5 + 2*sqrt(2))*(1 + sqrt(2))^n + (5 - 2*sqrt(2))*(1 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) with a(0) = 5, a(1) = 9 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 5;
    if (n == 1) return 9;
    seq_t a0 = 5;   /* a(n-2) */
    seq_t a1 = 9;   /* a(n-1) */
    seq_t an;       /* a(n)   */
    for (idx_t i = 2; i <= n; ++i) {
        an = 2 * a1 + a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

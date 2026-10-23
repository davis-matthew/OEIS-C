/*
 * A163306  a(n) = 12*a(n-1) - 31*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A163306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 31*a(n-2), with a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 7;
    seq_t a0 = 1; /* a(0) */
    seq_t a1 = 7; /* a(1) */
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = 12 * a1 - 31 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

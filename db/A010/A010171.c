/*
 * A010171  Continued fraction for sqrt(103).
 * https://oeis.org/A010171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(103); a(0)=10; for n>0,
 *            a(n)=P[(n-1)%12] where P = {6,1,2,1,1,9,1,1,2,1,6,20}
 * Range:     u8 n <= 999, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Periodic part of the continued fraction for sqrt(103) */
    static const seq_t period[12] = {
        6, 1, 2, 1, 1, 9,
        1, 1, 2, 1, 6, 20
    };

    if (n == 0) {
        return 10;
    }
    /* compute index modulo period length */
    idx_t idx = (n - 1) % 12;
    return period[idx];
}

/*
 * A226354  Squares that become cubes when their rightmost digit is removed.
 * https://oeis.org/A226354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   for n>11, a(n) = (100*(n-6)^3)^2 = 10000*(n-6)^6; a(1..11) are
 *            the exceptional initial terms
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 14, u64 n <= 356, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[11] = {
        1, 4, 9, 16, 81, 10000, 640000, 7290000,
        40960000, 156250000, 188210961
    };
    if (n <= 11) return init[n - 1];
    seq_t k = (seq_t)(n - 6);
    seq_t k2 = k * k;
    seq_t k4 = k2 * k2;
    seq_t k6 = k4 * k2;
    return k6 * ((seq_t)10000);
}

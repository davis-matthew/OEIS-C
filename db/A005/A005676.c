/*
 * A005676  a(n) = Sum_{k=0..n} C(n-k,4*k).
 * https://oeis.org/A005676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) + a(n-5), a(0) =
 *            a(1) = a(2) = a(3) = a(4) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x0 = 1, x1 = 1, x2 = 1, x3 = 1, x4 = 1;  /* a(i-5) .. a(i-1) */
    idx_t i;
    if (n < 5) return x4;
    for (i = 5; i <= n; i++) {
        seq_t pos = 4 * x4 + 4 * x2 + x0;
        seq_t neg = 6 * x3 + x1;
        seq_t nx = pos - neg;
        x0 = x1;
        x1 = x2;
        x2 = x3;
        x3 = x4;
        x4 = nx;
    }
    return x4;
}

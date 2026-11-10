/*
 * A091548  Alternating row sums of triangle A091543.
 * https://oeis.org/A091548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m=0..n-1} (-1)^m * A091543(n,m), where A091543(n,0)
 *            = 2^(n-1) and A091543(n,m) = (1/2)*Product_{j=0..n-m-1}
 *            (m*j+2)*(m*j+1) for m >= 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 8, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t p2 = 1;
    for (idx_t i = 1; i < n; i++) p2 *= 2;
    sum = p2;
    for (idx_t m = 1; m + 1 <= n; m++) {
        idx_t iter = n - m;
        /* j = 0 factor (2*1)/2 = 1 */
        seq_t prod = 1;
        for (idx_t j = 1; j < iter; j++) {
            seq_t x = (seq_t)m * (seq_t)j;
            prod *= (x + 2);
            prod *= (x + 1);
        }
        if (m & 1) sum -= prod;
        else sum += prod;
    }
    return sum;
}

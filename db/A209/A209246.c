/*
 * A209246  Row sums of triangle A196020.
 * https://oeis.org/A209246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1, k(k+1)/2 <= n, k | (n - k(k+1)/2)} (2*((n -
 *            k(k+1)/2)/k) + 1)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k;
    for (k = 1; k * (k + 1) / 2 <= n; k++) {
        idx_t diff = n - k * (k + 1) / 2;
        if (diff % k == 0) {
            sum += (seq_t)2 * (seq_t)(diff / k) + 1;
        }
    }
    return sum;
}

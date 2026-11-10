/*
 * A203749  v(n+1)/v(n), where v=A203748.
 * https://oeis.org/A203749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n-1} (v_i^2 + v_i*w + w^2), where v_0 = 0,
 *            v_i = floor((i+1)/2) for i >= 1, and w = v_n = floor((n+1)/2).
 *            This is v(n+1)/v(n) for v = A203748
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t w = (seq_t)((n + 1) / 2);
    seq_t res = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t x = (i == 0) ? (seq_t)0 : (seq_t)((i + 1) / 2);
        res = res * (x * x + x * w + w * w);
    }
    return res;
}

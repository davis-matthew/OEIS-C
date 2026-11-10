/*
 * A071908  A002487(n)*A002487(n+1)*A002487(n+2).
 * https://oeis.org/A071908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(n)*A002487(n+1)*A002487(n+2), where A002487(0)=0,
 *            A002487(1)=1, A002487(2n)=A002487(n),
 *            A002487(2n+1)=A002487(n)+A002487(n+1)
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A002487_a(idx_t n) {
    seq_t x = 1, y = 0;
    while (n > 0) {
        if (n & 1) y += x;
        else x += y;
        n >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    seq_t p = A002487_a(n);
    seq_t q = A002487_a(n + 1);
    seq_t r = A002487_a(n + 2);
    return p * q * r;
}

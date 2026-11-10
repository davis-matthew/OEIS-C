/*
 * A096407  a(n) = f(n)/f(n-2) where f(k) = A002665(k).
 * https://oeis.org/A096407
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) + b(n-1) + 1, where b = A002065: b(0)=0, b(k+1) =
 *            b(k)^2 + b(k) + 1. This is f(n)/f(n-2) for f = A002665 (offset
 *            2, a(2)=5)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = (seq_t)0;  /* b(k-1) */
    seq_t cur = (seq_t)0;   /* b(k) */
    for (idx_t k = 1; k <= n; k++) {
        seq_t nb = cur * cur + cur + (seq_t)1;
        prev = cur;
        cur = nb;
    }
    return cur + prev + (seq_t)1;
}

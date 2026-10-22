/*
 * A001040  a(n+1) = n*a(n) + a(n-1) with a(0)=0, a(1)=1.
 * https://oeis.org/A001040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, frac
 * Formula:   a(n) = (n-1)*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = 0;
    seq_t cur = 1;
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = (seq_t)(i - 1) * cur + prev;
        prev = cur;
        cur = next;
    }
    return cur;
}

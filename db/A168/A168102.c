/*
 * A168102  a(n) = sum of natural numbers m such that n - 3 <= m <= n + 3.
 * https://oeis.org/A168102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=max(0,n-3)}^{n+3} i For n >= 3: a(n) = 7*n For n <
 *            3: a(n) = (n+3)*(n+4)/2
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < (idx_t)3) {
        seq_t nn = (seq_t)n;
        return (nn + (seq_t)3) * (nn + (seq_t)4) / (seq_t)2;
    } else {
        return (seq_t)7 * (seq_t)n;
    }
}

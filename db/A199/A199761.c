/*
 * A199761  a(n) = (9*11^n+1)/5.
 * https://oeis.org/A199761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*11^n+1)/5. Recurrence: a(0)=2; a(n)=11*a(n-1)-2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    seq_t val = (seq_t)2;
    for (idx_t i = 0; i < n; ++i) {
        val = val * (seq_t)11 - (seq_t)2;
    }
    return val;
}

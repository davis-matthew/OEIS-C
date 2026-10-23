/*
 * A110595  a(1)=5. For n > 1, a(n) = 4*5^(n-1) = A005054(n).
 * https://oeis.org/A110595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, less, nonn
 * Formula:   a(1) = 5; a(n) = 4 * 5^(n-1) for n > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)5;
    }
    seq_t value = 1;
    idx_t i;
    for (i = 0; i < n - 1; ++i) {
        value *= (seq_t)5;
    }
    value *= (seq_t)4;
    return value;
}

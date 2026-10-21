/*
 * A231500  a(n) = Sum_{i=0..n} wt(i)^2, where wt(i) = A000120(i).
 * https://oeis.org/A231500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} popcount(i)^2, where popcount is the number
 *            of 1 bits in i
 * Range:     u8 n <= 34, u16 n <= 2020, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t bits = i;
        seq_t pc = 0;
        while (bits) {
            bits &= bits - 1;
            ++pc;
        }
        sum += pc * pc;
    }
    return sum;
}

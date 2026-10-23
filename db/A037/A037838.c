/*
 * A037838  a(n) = Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*6^i:
 *          i=0,1,...,m} is the base 6 representation of n.
 * https://oeis.org/A037838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1}^{m} |d_i - d_{i-1}|, where n = Σ_{i=0}^{m} d_i
 *            6^i is the base‑6 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    seq_t prev = n % 6;
    n /= 6;
    while (n > 0) {
        seq_t d = n % 6;
        sum += (prev > d ? prev - d : d - prev);
        prev = d;
        n /= 6;
    }
    return sum;
}

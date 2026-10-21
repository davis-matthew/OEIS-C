/*
 * A231684  a(n) = Sum_{i=0..n} digsum_9(i), where digsum_9(i) = A053830(i).
 * https://oeis.org/A231684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_9(i) where digsum_9(i) = sum of
 *            digits of i expressed in base 9
 * Range:     u8 n <= 42, u16 n <= 4522, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t x = i;
        unsigned int dsum = 0;
        while (x) {
            dsum += x % 9;
            x /= 9;
        }
        total += (seq_t)dsum;
    }
    return total;
}

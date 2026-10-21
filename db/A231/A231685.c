/*
 * A231685  a(n) = Sum_{i=0..n} digsum_9(i)^2, where digsum_9(i) = A053830(i).
 * https://oeis.org/A231685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 516, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A231685: a(n) = Sum_{i=0..n} digsum_9(i)^2 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t t = i;
        int ds = 0;
        while (t > 0) {
            ds += t % 9;
            t /= 9;
        }
        sum += (seq_t)ds * (seq_t)ds;
    }
    return sum;
}

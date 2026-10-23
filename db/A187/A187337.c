/*
 * A187337  a(n) = Sum_{k=1..6} floor(k*n/7).
 * https://oeis.org/A187337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..6} floor(k*n/7)
 * Range:     u8 n <= 86, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= 6; ++k) {
        sum += ( (seq_t)n * k ) / 7;
    }
    return sum;
}

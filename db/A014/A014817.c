/*
 * A014817  a(n) = Sum_{k=1..n} floor(k^2/n).
 * https://oeis.org/A014817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor(k^2 / n)
 * Range:     u8 n <= 15, u16 n <= 442, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        seq_t val = sq / (seq_t)n;
        sum += val;
    }
    return sum;
}

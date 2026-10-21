/*
 * A231668  a(n) = Sum_{i=0..n} digsum_5(i), where digsum_5(i) = A053824(i).
 * https://oeis.org/A231668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0..n} digsum_5(i) (digsum_5(i) = sum of base‑5
 *            digits of i)
 * Range:     u8 n <= 56, u16 n <= 6244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple linear scan. For n up to ~10 000 it is far below
   the 2‑second limit. All arithmetic is performed with width‐safe types, casting to the
   sequence type only when returning the result. */
#include <stdint.h>

static idx_t digit_sum_base5(idx_t x)
{
    idx_t sum = 0;
    while (x) {
        sum += x % 5;
        x   /= 5;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;        /* running sum of digsum_5(i) for i = 0..n */
    for (idx_t i = 0; i <= n; ++i) {
        total += (seq_t)digit_sum_base5(i);
    }
    return total;
}

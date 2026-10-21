/*
 * A222548  a(n) = Sum_{k=1..n} floor(n/k)^2.
 * https://oeis.org/A222548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^2
 * Range:     u8 n <= 13, u16 n <= 201, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the OEIS sequence A222548.
 * a(n) = Sum_{k=1..n} floor(n/k)^2   for n >= 1.
 * Returns 0 for n == 0 (out of range).
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* out of range, return 0 */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t q = (seq_t)(n / k);     /* floor(n/k) as seq_t */
        sum += q * q;
    }
    return sum;
}

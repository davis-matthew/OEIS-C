/*
 * A169813  a(n) = n XOR sigma(n), where sigma(n) is the number of divisors of
 *          n, A000203.
 * https://oeis.org/A169813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n XOR σ(n), where σ(n) is the sum of divisors of n
 *            (A000203)
 * Range:     u8 n <= 107, u16 n <= 9635, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sum_div = sigma_of(n);
    return (seq_t)n ^ sum_div;
}

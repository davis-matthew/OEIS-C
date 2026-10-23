/*
 * A171462  Number of hands a bartender needs to have in order to win at the
 *          blind bartender's problem with n glasses in a cycle.
 * https://oeis.org/A171462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - n / p, where p = largest prime divisor of n; a(1) = 0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    uint64_t p = largest_prime_divisor((uint64_t)n);
    seq_t tn = (seq_t)n;
    return tn - tn / (seq_t)p;
}

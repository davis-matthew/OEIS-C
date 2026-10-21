/*
 * A076563  a(n) = n - greatest prime divisor of n, for n>1.
 * https://oeis.org/A076563
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - largest_prime_divisor(n)
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* offset starts at 2; below is undefined */
    seq_t n_seq = (seq_t)n;
    seq_t lpf = (seq_t)largest_prime_divisor(n); // largest prime factor of n
    return n_seq - lpf;
}

/*
 * A076566  Greatest prime divisor of 3n+3 (sum of three successive integers).
 * https://oeis.org/A076566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max(3, largest_prime_factor(n+1))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   local_largest_prime_factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest prime divisor of 3n+3 (eg. sum of three successive integers) */
#include <stdint.h>
#include "../utils/local_largest_prime_factor_uint64.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence.  For robustness we return 0 for n==0. */
    if (n == 0) return (seq_t)0;

    uint64_t m = (uint64_t)(n + 1);          /* 3n+3 = 3*(n+1), we need LPF of (n+1) */
    seq_t lpf = local_largest_prime_factor_uint64(m);
    return (lpf >= (seq_t)3) ? lpf : (seq_t)3;
}

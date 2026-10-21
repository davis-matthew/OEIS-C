/*
 * A078897  Number of times the greatest prime factor of n is a factor in all
 *          numbers <=n; a(1)=1.
 * https://oeis.org/A078897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v_gpf(n)(n!) – the exponent of the greatest prime factor
 *            of n in the factorial n!
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_divisor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    uint64_t p = largest_prime_divisor(n);
    seq_t sum = 0;
    idx_t power = n / p;
    while (power)
    {
        sum += (seq_t)power;
        power /= p;
    }
    return sum;
}

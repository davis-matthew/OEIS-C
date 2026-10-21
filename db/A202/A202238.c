/*
 * A202238  Characteristic function of positive integers not prime and not a
 *          power of 2.
 * https://oeis.org/A202238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is not prime and not a power of 2; otherwise a(n)
 *            = 0
 * Range:     u8 n <= 9735, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   is_power_of_two, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return 0;
    if (is_power_of_two((seq_t)n))
        return 0;
    return 1;
}

/*
 * A068395  a(n) = n-th prime minus its sum of digits.
 * https://oeis.org/A068395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, base, changed
 * Formula:   a(n) = get_prime(n) - digit_sum(get_prime(n))
 * Range:     u8 n <= 54, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by requirement */
    uint32_t p = get_prime(n);
    return (seq_t)p - digit_sum((seq_t)p);
}

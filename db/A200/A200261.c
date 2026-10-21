/*
 * A200261  a(n) = 1 iff n-th prime has an even digit sum.
 * https://oeis.org/A200261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 iff the sum of the digits of the n-th prime is even,
 *            otherwise 0
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    uint32_t p = get_prime(n);            /* n-th prime */
    seq_t s = digit_sum(p);              /* digit sum of that prime */
    return ((s & (seq_t)1) == 0) ? (seq_t)1 : (seq_t)0;
}

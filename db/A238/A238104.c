/*
 * A238104  Sum of digits in periodic part of decimal expansion of 1/prime(n).
 * https://oeis.org/A238104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of the periodic part of 1/prime(n)
 * Range:     u8 n <= 16, u16 n <= 1709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);              /* nth prime */
    if (p == 2 || p == 5) return 0;          /* terminating decimals */

    uint64_t rem = 1;
    seq_t sum = 0;
    do {
        rem *= 10;
        uint64_t digit = rem / p;
        sum += (seq_t)digit;                /* digit is 0..9 */
        rem %= p;
    } while (rem != 1);

    return sum;
}

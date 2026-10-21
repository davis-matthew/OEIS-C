/*
 * A239693  Base 7 sum of digits of prime(n).
 * https://oeis.org/A239693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of base‑7 digits of the n‑th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    uint32_t p = get_prime(n);   /* nth prime (1‑based) */
    seq_t sum = 0;

    /* sum of base‑7 digits */
    while (p > 0) {
        sum += (seq_t)(p % 7);
        p /= 7;
    }

    return sum;
}

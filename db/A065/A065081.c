/*
 * A065081  Alternating bit sum (A065359) for n-th prime p: replace 2^k with
 *          (-1)^k in binary expansion of p.
 * https://oeis.org/A065081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = \sum_{k\ge0} bit_k(p) * (-1)^k, where p is the nth prime
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth prime is obtained via get_prime. */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);      /* nth prime (p ≥ 2) */
    seq_t   sum = 0;                /* alternating bit sum */
    seq_t   sign = 1;                /* (-1)^k, start with k = 0 */

    while (p != 0) {
        if (p & 1U) sum += sign;     /* add or subtract the current bit */
        sign = -sign;                /* flip sign for next bit */
        p >>= 1;                     /* move to next less significant bit */
    }

    return sum;
}

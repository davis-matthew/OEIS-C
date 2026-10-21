/*
 * A065073  a(n) = prime(n) + (sum of digits of prime(n)).
 * https://oeis.org/A065073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = p(n) + digit_sum(p(n))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);   /* nth prime (1‑based) */
    seq_t ds = digit_sum(p);        /* sum of decimal digits of the prime */
    return p + ds;
}

/*
 * A181149  a(n) = prime(n)^3 + prime(n)^2 + prime(n).
 * https://oeis.org/A181149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^3 + prime(n)^2 + prime(n)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1-based: get_prime(1) returns the first prime 2 */
    uint32_t p = get_prime(n);
    seq_t pn = (seq_t)p;
    seq_t sq = pn * pn;          /* p^2 */
    return sq * pn + sq + pn;    /* p^3 + p^2 + p */
}

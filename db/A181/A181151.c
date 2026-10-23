/*
 * A181151  a(n) = prime(n)^3 + prime(n)^2 + 1.
 * https://oeis.org/A181151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^3 + prime(n)^2 + 1
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1-based) */
    uint32_t p = get_prime(n);
    seq_t pp = (seq_t)p;
    /* compute pp^3 + pp^2 + 1 */
    return pp * pp * pp + pp * pp + (seq_t)1;
}

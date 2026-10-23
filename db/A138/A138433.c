/*
 * A138433  a(n) = ((n-th prime)^5-(n-th prime)^3)/3.
 * https://oeis.org/A138433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-th prime)^5 - (n-th prime)^3)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    seq_t pseq = (seq_t)p;
    seq_t p2   = pseq * pseq;
    seq_t p3   = p2 * pseq;
    seq_t p5   = p3 * p2;
    seq_t res  = (p5 - p3) / (seq_t)3;
    return res;
}

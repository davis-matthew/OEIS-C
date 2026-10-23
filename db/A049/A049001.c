/*
 * A049001  a(n) = prime(n)^2 - 2.
 * https://oeis.org/A049001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 - 2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
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

    /* compute p^2 - 2 in type seq_t */
    seq_t sp = (seq_t)p;
    return sp * sp - (seq_t)2;
}

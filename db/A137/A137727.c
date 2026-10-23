/*
 * A137727  Final digit of prime(n)*prime(n+1).
 * https://oeis.org/A137727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (prime(n) * prime(n+1)) mod 10
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
    /* n >= 1 as per OEIS definition */
    uint32_t p1 = get_prime(n);
    uint32_t p2 = get_prime(n + 1);
    /* product modulo 10 computed from last digits only */
    seq_t d1 = (seq_t)(p1 % 10);
    seq_t d2 = (seq_t)(p2 % 10);
    return (d1 * d2) % 10;
}

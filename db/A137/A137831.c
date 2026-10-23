/*
 * A137831  a(n) = (prime(n)^2 minus its last digit)/20.
 * https://oeis.org/A137831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (prime(n)^2 - lastdigit(prime(n)^2))/20
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;
    seq_t sq = p * p;
    seq_t last = sq % (seq_t)10;
    return (sq - last) / (seq_t)20;
}

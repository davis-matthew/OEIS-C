/*
 * A131377  a(n) = (pi(n)+1) mod 2.
 * https://oeis.org/A131377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (pi(n) + 1) mod 2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    seq_t pi = prime_pi(n);
    return (pi + 1) & 1;
}

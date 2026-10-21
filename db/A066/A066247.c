/*
 * A066247  Characteristic function of composite numbers: 1 if n is composite
 *          else 0.
 * https://oeis.org/A066247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is composite (n>1 and not prime), else 0
 * Range:     u8 n <= 8715, u16 n <= 8715, u32 n <= 8715, u64 n <= 8715,
 *            u128 n <= 8715, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    return (seq_t)(is_prime(n) ? 0 : 1);
}

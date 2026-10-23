/*
 * A135182  p^5 + p^3 + p^2. Exponents are prime numbers and p = prime(n).
 * https://oeis.org/A135182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n)^5 + p(n)^3 + p(n)^2, offset 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 914, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; undefined for n=0 */
    seq_t p = (seq_t)get_prime(n);
    seq_t p2 = p * p;
    seq_t p3 = p2 * p;
    seq_t p5 = p3 * p2;              /* p^5 = p^3 * p^2 */
    return p5 + p3 + p2;
}

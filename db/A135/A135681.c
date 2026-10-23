/*
 * A135681  a(n)=n if n=prime. Otherwise, a(n)=4 if n is even and a(n)=1 if n
 *          is odd.
 * https://oeis.org/A135681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = n if n is prime; * 4 if n is even and composite; * 1 if n
 *            is odd and composite
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)n;

    /* n is composite; decide by parity */
    return ((n & 1ULL) == 0) ? (seq_t)4 : (seq_t)1;
}

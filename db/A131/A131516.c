/*
 * A131516  a(n)=1 if n is an odd prime number, otherwise, a(n)=n.
 * https://oeis.org/A131516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is an odd prime, a(n) = n otherwise
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 1 are handled by the general rule below */
    if ((n & 1ULL) == 0)          /* even numbers (including 0) */
        return (seq_t)n;
    /* n is odd; check primality */
    if (is_prime(n))
        return (seq_t)1;
    return (seq_t)n;
}

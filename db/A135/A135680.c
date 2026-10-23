/*
 * A135680  a(n) = n if n = 1 or if n is prime. Otherwise, n = 4 if n is even
 *          and n = 5 if n is odd.
 * https://oeis.org/A135680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n == 1 or prime; 4 if even composite; 5 if odd
 *            composite
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (is_prime(n))
        return (seq_t)n;
    if ((n & 1) == 0)
        return (seq_t)4;
    return (seq_t)5;
}

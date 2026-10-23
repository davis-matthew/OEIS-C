/*
 * A145737  a(n) = square part of A145609(n).
 * https://oeis.org/A145737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n+1 if 2n+1 is prime, 1 otherwise, for n > 1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)        /* a(1) = 1 by definition */
        return (seq_t)1;
    idx_t m = 2 * n + 1;        /* candidate prime 2n+1 */
    return (seq_t)(is_prime(m) ? m : 1);
}

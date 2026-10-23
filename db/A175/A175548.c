/*
 * A175548  Binary weight of sigma(n).
 * https://oeis.org/A175548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary weight of sigma(n)
 * Range:     u8 n <= 429, u16 n <= 9103, u32 n <= 9103, u64 n <= 9103,
 *            u128 n <= 9103, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);     /* sum of divisors of n */
    seq_t count = 0;
    while (s != 0) {
        if ((s & 1) != 0) ++count;
        s >>= 1;
    }
    return count;
}

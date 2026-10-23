/*
 * A146969  a(1)=3, a(2)=1, a(n) = (n+1)/2 if n is an odd prime, and a(n) =
 *          a(floor(n/2)) otherwise.
 * https://oeis.org/A146969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (n+1)/2 if n is an odd prime, a(n) = a(floor(n/2))
 *            otherwise; a(1) = 3, a(2) = 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    while (1) {
        if (n == 1) return 3;
        if (n == 2) return 1;
        if ((n & 1ULL) && is_prime(n))
            return ((seq_t)n + 1) / 2;
        n >>= 1;
    }
}

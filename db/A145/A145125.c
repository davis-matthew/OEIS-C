/*
 * A145125  Primes of the form 4n^2-4n+653.
 * https://oeis.org/A145125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 4k^2-4k+653, where k>=0
 * Range:     u8 none, u16 n <= 87, u32 n <= 72, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence offset: 1  (a(1)=653) */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t k = 0;          /* k starts at 0 */

    while (true)
    {
        seq_t val = 4 * k * k - 4 * k + 653;
        if (is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }
        ++k;
    }
}

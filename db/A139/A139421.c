/*
 * A139421  a(1)=1; for n>1, a(n) = largest prime divisor of n!!.
 * https://oeis.org/A139421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime divisor of n!! (double factorial)
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
    if (n == 1) return 1;
    if (n == 2) return 2;

    idx_t m = (n & 1) ? n : n >> 1;   /* m = n for odd, n/2 for even */

    for (idx_t x = m; x >= 2; --x)
    {
        if (is_prime(x))
            return (seq_t)x;
    }
    return 1; /* should never reach here */
}

/*
 * A101776  Smallest k such that k^2 is equal to the sum of n
 *          not-necessarily-distinct primes plus 1.
 * https://oeis.org/A101776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(2*n+1))
 * Range:     u8 n <= 195, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    seq_t t = (seq_t)2 * (seq_t)n + 1;      /* t = 2*n + 1 */
    seq_t s = isqrt(t);                    /* floor sqrt(t) */
    return (s * s == t) ? s : s + 1;       /* ceil sqrt(t) */
}

/*
 * A000132  Number of ways of writing n as a sum of 5 squares.
 * https://oeis.org/A000132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = A000118(n) + 2*Sum_{k=1..floor(sqrt(n))} A000118(n -
 *            k^2), where A000118(0) = 1 and A000118(m) = 8*sigma(m) -
 *            32*sigma(m/4) for m >= 1 (the second term only when 4 | m)
 * Range:     u8 n <= 6, u16 n <= 234, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   r4, sigma_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/r4.h"
#include "../utils/sigma_u64.h"

seq_t a(idx_t n)
{
    seq_t s = r4((uint64_t)n);
    uint64_t k;
    for (k = 1; k * k <= (uint64_t)n; k++)
        s += (seq_t)2 * r4((uint64_t)n - k * k);
    return s;
}

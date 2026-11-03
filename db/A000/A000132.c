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
 * Validated: standard, pass, 63 terms through n = 9587 (2026-11-01)
 * Helpers:   sigma_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_u64_to_u64.h"

static seq_t r4(uint64_t m)
{
    seq_t r;
    if (m == 0) return (seq_t)1;
    r = (seq_t)8 * (seq_t)sigma_u64_to_u64(m);
    if (m % 4 == 0)
        r -= (seq_t)32 * (seq_t)sigma_u64_to_u64(m / 4);
    return r;
}

seq_t a(idx_t n)
{
    seq_t s = r4((uint64_t)n);
    uint64_t k;
    for (k = 1; k * k <= (uint64_t)n; k++)
        s += (seq_t)2 * r4((uint64_t)n - k * k);
    return s;
}

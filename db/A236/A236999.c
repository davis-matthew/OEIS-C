/*
 * A236999  Odd part of n*(n+3)/2-1 (A034856).
 * https://oeis.org/A236999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part(n*(n+3)/2 - 1)
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;
    seq_t val  = ns * (ns + (seq_t)3) / 2 - 1;
    return odd_part(val);
}

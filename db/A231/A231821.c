/*
 * A231821  a(n) = mu(n) + 3, where mu is the Mobius function (A008683).
 * https://oeis.org/A231821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = mobius(n) + 3, for n >= 1
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* The Mobius function μ(n) is defined for all positive integers n.
       It returns -1, 0, or 1. Adding 3 yields the sequence A231821. */
    return mobius(n) + (seq_t)3;
}

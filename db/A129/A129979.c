/*
 * A129979  a(n) = 2-mu(n), where mu=A008683 is the Moebius function.
 * https://oeis.org/A129979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - mu(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided mobius routine */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    return (seq_t)2 - mobius(n);
}

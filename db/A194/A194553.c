/*
 * A194553  Centered cube numbers: (n+1)^25 + n^25.
 * https://oeis.org/A194553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^25 + (n+1)^25
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t ns1 = ns + (seq_t)1;
    seq_t p1 = big_pow(ns, (seq_t)25);
    seq_t p2 = big_pow(ns1, (seq_t)25);
    return p1 + p2;
}

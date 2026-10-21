/*
 * A052699  Expansion of e.g.f. (1+x^5-x^6)/(1-x).
 * https://oeis.org/A052699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! for n != 5; a(5) = 2 * 5! (offset 0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 5)
        return factorial(5) * (seq_t)2;
    return factorial(n);
}

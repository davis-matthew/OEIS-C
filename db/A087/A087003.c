/*
 * A087003  a(2n) = 0 and a(2n+1) = mu(2n+1); also the sum of Mobius function
 *          values computed for terms of 3x+1 trajectory started at n,
 *          provided that Collatz conjecture is true.
 * https://oeis.org/A087003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = 0 if n is even, otherwise a(n) = μ(n) (the Möbius
 *            function)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius_uint64.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return 0;
    return mobius_uint64(n);
}

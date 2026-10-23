/*
 * A036239  Number of 2-element intersecting families of an n-element set;
 *          number of 2-way interactions when 2 subsets of power set on {1..n}
 *          are chosen at random.
 * https://oeis.org/A036239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/2)*(4^n - 3^n - 2^n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the powers using the helper seq_pow. */
    seq_t p4 = seq_pow((seq_t)4, n);
    seq_t p3 = seq_pow((seq_t)3, n);
    seq_t p2 = seq_pow((seq_t)2, n);

    /* Apply the formula: a(n) = (4^n - 3^n - 2^n + 1) / 2. */
    seq_t tmp = (p4 - p3 - p2 + 1) / 2;
    return tmp;
}

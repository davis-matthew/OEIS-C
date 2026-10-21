/*
 * A219167  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and columns,
 *          0..2 n X 2 array.
 * https://oeis.org/A219167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 a(n) = (n⁴)/8 - (3·n³)/4 + (55·n²)/8 - (49·n)/4 + 8 for
 *            n > 1
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)3;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t res = 0;
    seq_t t;

    t = n4;            /* n⁴ */
    t /= (seq_t)8;
    res += t;

    t = (seq_t)3 * n3;
    t /= (seq_t)4;
    res -= t;

    t = (seq_t)55 * n2;
    t /= (seq_t)8;
    res += t;

    t = (seq_t)49 * nn;
    t /= (seq_t)4;
    res -= t;

    res += (seq_t)8;

    return res;
}

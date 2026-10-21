/*
 * A065801  Least k such that n^k > 2^n.
 * https://oeis.org/A065801
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( n / log₂(n) ) (least k with n^k > 2^n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>  /* log2l, floorl */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* values for n < 2 are irrelevant */

    long double nd = (long double)n;
    long double log2n = log2l(nd);                  /* log base 2 of n */
    long double x = nd / log2n;                    /* n / log₂(n) */

    /* add a tiny epsilon to avoid rounding down when x is very close to an integer */
    const long double EPS = 1e-12L;
    uint64_t floorx = (uint64_t)floorl(x + EPS);

    return (seq_t)(floorx + 1);                     /* ceil(x) */
}

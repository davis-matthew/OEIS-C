/*
 * A038210  Triangle whose (i,j)-th entry is binomial(i,j)*2^(i-j)*4^j.
 * https://oeis.org/A038210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i,j) * 2^(i+j) where i and j satisfy n =
 *            i*(i+1)/2 + j, with 0 <= j <= i. (Triangle entry T(i,j) of the
 *            triangle defined in OEIS A038210.)
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t i = 0;
    idx_t t = 0;              /* t = i*(i+1)/2 */
    while (t + i + 1 <= n) {  /* find largest i with t <= n */
        t += i + 1;
        ++i;
    }
    idx_t j = n - t;          /* column index in row i */

    seq_t val = binomial(i, j);
    val *= pow2(i + j);       /* 2^(i+j) */
    return val;
}

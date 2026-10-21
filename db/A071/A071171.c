/*
 * A071171  L_2 norm of vertices of Permuto-Associahedron in R^n.
 * https://oeis.org/A071171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1, 3)*(30*n^4-33*n^2+2)/70
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 34, u64 n <= 824, u128 n <= 1000,
 *            big any n
 * Helpers:   binom, muldiv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/muldiv.h"

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n4  = n2 * n2;
    seq_t poly = (seq_t)30 * n4 - (seq_t)33 * n2 + 2;
    seq_t bin  = binom(n + 1, 3);        /* binomial coefficient */
    return muldiv(bin, poly, 70);        /* perform a* b / 70 safely */
}

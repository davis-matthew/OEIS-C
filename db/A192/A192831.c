/*
 * A192831  Molecular topological indices of the hypercube graphs.
 * https://oeis.org/A192831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n^2 * (1 + 2^(n-1)) (Offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to the result type */
    seq_t nseq  = (seq_t)n;          /* n as seq_t */
    seq_t n2    = nseq * nseq;       /* n^2 */

    /* Powers of two */
    seq_t two_n  = pow2(n);          /* 2^n */
    seq_t two_nm1 = pow2(n - 1);     /* 2^(n-1) */

    /* Compute the formula */
    seq_t term   = (seq_t)1 + two_nm1;   /* 1 + 2^(n-1) */
    seq_t result = two_n * n2 * term;    /* 2^n * n^2 * (1 + 2^(n-1)) */

    return result;
}

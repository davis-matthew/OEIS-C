/*
 * A227702  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially in Fig. 1 of the Iranmanesh et al. reference.
 * https://oeis.org/A227702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 22, u128 n <= 53,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (OEIS A227702):
   a(n) = 167682 + 2^n*(79709 + 24876*n + 3996*n^2)
               + 4^n*(13458 + 1512*n + 1296*n^2)
   (offset: a(1) is first term) */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0 */
    if (n < 1)
        return 0;

    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;

    /* Compute the polynomial terms */
    seq_t t1 = (seq_t)3996 * n2 + (seq_t)24876 * nseq + 79709;
    seq_t t2 = (seq_t)1296 * n2 + (seq_t)1512 * nseq + 13458;

    /* Powers of 2 and 4 */
    seq_t pow2n = pow_seq((seq_t)2, (int)n);
    seq_t pow4n = pow_seq((seq_t)4, (int)n);

    /* Assemble the result */
    seq_t result = 167682 + pow2n * t1 + pow4n * t2;
    return result;
}

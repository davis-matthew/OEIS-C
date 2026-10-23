/*
 * A037966  a(n) = n^2*binomial(2*n-2, n-1).
 * https://oeis.org/A037966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * binomial(2*n-2, n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

/* External linkage function computing the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t k   = n - 1;          /* n-1 (>=0 for n>=1) */
    idx_t top = 2 * k;          /* 2*n-2 */

    seq_t bin = binomial(top, k);          /* binomial(2*n-2, n-1) */
    seq_t nseq = (seq_t)n;                 /* n as seq_t */

    seq_t prod = seq_mul(bin, nseq);       /* bin * n */
    prod = seq_mul(prod, nseq);            /* bin * n^2 */

    return prod;
}

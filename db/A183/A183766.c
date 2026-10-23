/*
 * A183766  1/32 the number of (2n+1) X 4 binary arrays with equal numbers of
 *          2 X 2 subblocks with sum mod two being 0 and 1.
 * https://oeis.org/A183766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) * binomial(6*n, 3*n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   binom, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility headers that provide the required big‑integer operations. */
#include "../utils/binom.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* Computes the n-th term of sequence A183766.  The offset of the sequence
   is 1, so a(1) corresponds to the first term listed in the OEIS entry. */
seq_t a(idx_t n)
{
    /* Guard against the undefined behaviour for n = 0 (offset is 1). */
    if (n == 0)
        return 0;

    /* binomial coefficient binom(6*n , 3*n) */
    seq_t bin = binom(6 * n, 3 * n);

    /* 2^(2*n-1) */
    seq_t pow2 = seq_pow((seq_t)2, 2 * n - 1);

    /* a(n) = bin * 2^(2*n-1) */
    return seq_mul(bin, pow2);
}

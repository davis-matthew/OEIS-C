/*
 * A079321  Composite numbers of the form 1^1 * 2^2 * 3^3 * 4^4 * ... * n^n +
 *          1.
 * https://oeis.org/A079321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1}^{n+3} k^k + 1 (OEIS A079321)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seqpow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seqpow.h"

/* External function computing the n‑th term of A079321. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n = 0 return 1 (not used). */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    idx_t up_to = n + 3;             /* last factor index */

    for (idx_t k = 1; k <= up_to; ++k) {
        prod *= seqpow((seq_t)k, k); /* multiply by k^k */
    }

    return prod + (seq_t)1;          /* add the final +1 */
}

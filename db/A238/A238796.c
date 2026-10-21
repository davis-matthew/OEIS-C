/*
 * A238796  Symmetric (0,1)-matrices of order n where the numbers of 1's is
 *          equal to the order n.
 * https://oeis.org/A238796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n, 2k) * binomial(binomial(n,2),
 *            k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* C = binomial(n,2) = n*(n-1)/2 */
    idx_t C = (idx_t)((seq_t)n * ((seq_t)n - 1) / 2);

    seq_t result = 0;
    idx_t maxk = n >> 1;                   /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n, k << 1);    /* binomial(n, 2k) */
        seq_t t2 = binomial(C, k);         /* binomial(C, k)   */
        result += t1 * t2;
    }
    return result;
}

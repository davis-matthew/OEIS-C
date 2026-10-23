/*
 * A146205  Number of paths of the simple random walk on condition that the
 *          median applied to the partial sums S_0=0, S_1,...,S_n, n odd (n=15
 *          in this example), is equal to half-integer values k+1/2,
 *          -[n/2]-1<=k<=[n/2].
 * https://oeis.org/A146205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = 35 * binom(7, floor(n/2))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    if (k > 7U)
        return (seq_t)0;
    return (seq_t)35 * binom((idx_t)7, k);
}

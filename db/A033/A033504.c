/*
 * A033504  a(n)/4^n is the expected number of tosses of a coin required to
 *          obtain n+1 heads or n+1 tails.
 * https://oeis.org/A033504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = (n+1) * (2^(2*n+1) - binomial(2*n+1, n+1))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^(2*n+1) */
    seq_t pw = pow2(2 * n + 1);

    /* Compute binomial(2*n+1, n+1) */
    seq_t bc = binomial(2 * n + 1, n + 1);

    /* Difference */
    seq_t diff = pw - bc;

    /* Multiply by (n+1) */
    return diff * (seq_t)(n + 1);
}

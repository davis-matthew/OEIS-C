/*
 * A073389  Third column of the convolution triangle A073387(n+2, 2), for n >=
 *          0.
 * https://oeis.org/A073389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073389
 *
 * Formula: a(n) = Sum_{k=0..floor(n/2)} binomial(n-k+2, 2) *
 *                                binomial(n-k, k) *
 *                                2^(n-k)      .
 *
 * This implementation uses the binary coefficient routine binom()
 * and the power‑of‑two routine pow2() from the utils library.
 * The sum is accumulated in a seq_t type to avoid overflow in all
 * supported build configurations.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* accumulate the sum in a seq_t variable */
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t nk = n - k;                      /* n - k   */
        /* binomial(n-k+2, 2) = (nk+2)*(nk+1)/2 */
        seq_t bin1 = ((seq_t)(nk + 2) * (seq_t)(nk + 1)) / (seq_t)2;
        /* binomial(n-k, k) */
        seq_t bin2 = binom(nk, k);
        /* 2^(n-k) */
        seq_t pow = pow2(nk);

        /* add bin1 * bin2 * pow to the total */
        sum += bin1 * bin2 * pow;
    }

    return sum;
}

/*
 * A092443  Sequence arising from enumeration of domino tilings of Aztec
 *          Pillow-like regions.
 * https://oeis.org/A092443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * C(2n‑1, n‑1) + C(2n, n) (for n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is defined only for n ≥ 1 */
        return 0;

    /* first term: n * binomial(2n‑1, n‑1)                       */
    idx_t two_n_minus1 = 2 * n - 1;
    seq_t term1 = binomial(two_n_minus1, n - 1);
    term1 *= (seq_t)n;

    /* second term: binomial(2n, n)                              */
    seq_t term2 = binomial(2 * n, n);

    return term1 + term2;
}

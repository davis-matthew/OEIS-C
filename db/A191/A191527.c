/*
 * A191527  Number of turns in all left factors of Dyck paths of length n.
 * https://oeis.org/A191527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k * binomial(floor((n-1)/2), floor(k/2)) *
 *            binomial(ceil((n-1)/2), ceil(k/2))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* a(0) = 0, a(1) = 0 */
    if (n <= 1) {
        return 0;
    }

    /* Compute m1 = floor((n-1)/2), m2 = ceil((n-1)/2) */
    idx_t m1 = (n - 1) / 2;           /* floor((n-1)/2) */
    idx_t m2 = (n - 1) - m1;          /* ceil((n-1)/2) */

    seq_t result = 0;

    /* Sum over k = 1..n (k=0 contributes zero) */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t f = k / 2;            /* floor(k/2) */
        idx_t t = k - f;            /* ceil(k/2) */

        seq_t bin1 = binom(m1, f);
        seq_t bin2 = binom(m2, t);

        seq_t term = bin1 * bin2;
        term *= (seq_t)k;           /* multiply by k */

        result += term;
    }

    return result;
}

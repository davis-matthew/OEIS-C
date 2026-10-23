/*
 * A117902  Expansion of (1-x^2-2x^3)/(1-4x^3).
 * https://oeis.org/A117902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1 a(n) = 0 if n % 3 == 1 a(3k) = 2^(2k-1) for k >= 1
 *            a(3k+2) = -2^(2k) for k >= 0
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 47, i64 n <= 95, i128 n <= 191,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* fast path for the only non-zero values that are not covered by
       the general formulas */
    if (n == 0U)
        return 1;

    idx_t mod3 = n % 3U;

    /* terms where the sequence is zero */
    if (mod3 == 1U)
        return 0;

    if (mod3 == 2U) {                /* n = 3k + 2, k >= 0 */
        idx_t k = (n - 2U) / 3U;
        seq_t v = pow2(2 * k);      /* 2^(2k) */
        return -v;                  /* negative sign */
    }

    /* mod3 == 0, so n = 3k, k >= 1 */
    idx_t k = n / 3U;                /* k >= 1 because n != 0 */
    seq_t v = pow2(2 * k - 1);       /* 2^(2k-1) */
    return v;
}

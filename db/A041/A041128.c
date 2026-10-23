/*
 * A041128  Numerators of continued fraction convergents to sqrt(73).
 * https://oeis.org/A041128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n‑th convergent of sqrt(73). The
 *            continued fraction of sqrt(73) is [8; 1,1,5,5,1,1,16,
 *            1,1,5,5,1,1,16, …] The numerators p_k satisfy p_-2 = 0, p_-1 =
 *            1, p_k = A_k * p_{k-1} + p_{k-2} where A_0 = 8 and for k ≥ 1 the
 *            values A_k repeat the pattern (1,1,5,5,1,1,16). Thus...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t period[] = { (seq_t)1, (seq_t)1, (seq_t)5,
                                 (seq_t)5, (seq_t)1, (seq_t)1,
                                 (seq_t)16 };
static const size_t period_len = sizeof(period) / sizeof(period[0]);

seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)0;   /* p_-2  */
    seq_t prev1 = (seq_t)1;   /* p_-1  */
    seq_t cur;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ak;
        if (i == 0)
            ak = (seq_t)8;                                 /* A_0   */
        else
            ak = period[(size_t)(i - 1) % period_len];     /* A_i   */

        cur = ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* p_n  */
}

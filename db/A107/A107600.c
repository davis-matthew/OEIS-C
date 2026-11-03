/*
 * A107600  Column 5 of array illustrated in A089574 and related to A034261.
 * https://oeis.org/A107600
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{i=0}^{5} w[i] * C(n - i - 3, 6) where w = {1, 11, -4,
 *            -7, 5, -1}. (C denotes binomial coefficient) This follows from
 *            the generating function (x^5 - 5x^4 + 7x^3 + 4x^2 - 11x - 1) *
 *            x^9 / (x - 1)^7
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 39 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(k,6) = k*(k-1)*...*(k-5)/720.
   For k < 6 the result is 0.  */
static seq_t binom6(idx_t k)
{
    if (k < 6)
        return 0;
    unsigned __int128 num = 1;
    num *= (unsigned __int128)(k);
    num *= (unsigned __int128)(k - 1);
    num *= (unsigned __int128)(k - 2);
    num *= (unsigned __int128)(k - 3);
    num *= (unsigned __int128)(k - 4);
    num *= (unsigned __int128)(k - 5);
    num /= 720;                     /* 720 = 6! */
    return (seq_t)num;
}

seq_t a(idx_t n)
{
    if (n < 9)              /* offset is 9; behaviour below is irrelevant */
        return 0;

    static const int w[6] = {1, 11, -4, -7, 5, -1};
    seq_t result = 0;

    for (int i = 0; i < 6; ++i) {
        idx_t k = n - i - 3;
        if (k < 6)          /* C(k,6) is zero for k < 6 */
            continue;

        seq_t bin = binom6(k);
        int wi = w[i];
        if (wi > 0)
            result += bin * (seq_t)wi;
        else
            result -= bin * (seq_t)(-wi);
    }

    return result;
}

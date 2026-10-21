/*
 * A213289  Number of 10-length words w over n-ary alphabet such that for
 *          every prefix z of w we have #(z,a_i) = 0 or #(z,a_i) >= #(z,a_j)
 *          for all j>i and #(z,a_i) counts the occurrences of the i-th letter
 *          in z.
 * https://oeis.org/A213289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{9} p_k * binom(n+9-k,10) where p = {1, 312,
 *            1670, -1255, 15327, 38264, 81248, 406785, 520730, 2565718}
 *            (equivalent to the OEIS polynomial a(n) = n*(-6044030
 *            +17209877*n -19851310*n^2 +12422410*n^3 -4715472*n^4
 *            +1139127*n^5 -176832*n^6 +17190*n^7 -960*n^8 +24*n^9)/24)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 88, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    static const int coeffs[10] = {
        1, 312, 1670, -1255,
        15327, 38264, 81248, 406785,
        520730, 2565718
    };

    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t k = 0; k < 10; ++k) {
        idx_t top = n + 9 - k;
        if (top < 10)          /* binomial(n,10) is zero if n<10 */
            continue;
        seq_t bin = binomial(top, 10);
        int c = coeffs[(size_t)k];
        if (c >= 0)
            pos += (seq_t)c * bin;
        else
            neg += (seq_t)(-c) * bin;
    }
    return pos - neg;          /* guarantees result is non‑negative */
}

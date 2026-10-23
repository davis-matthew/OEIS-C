/*
 * A108675  a(n) = (n+1)*(n+2)*(61*n^4 + 366*n^3 + 845*n^2 + 888*n + 360)/720.
 * https://oeis.org/A108675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(61*n^4 + 366*n^3 + 845*n^2 + 888*n +
 *            360)/720 Alternative binomial representation: a(n) = Σ_{i=0}^{4}
 *            c[i] * binom(n+6-i, 6) with c = [1, 14, 31, 14, 1]
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const idx_t coeff[5] = {1, 14, 31, 14, 1};
    seq_t result = 0;
    for (int i = 0; i < 5; ++i) {
        idx_t N = n + 6ULL - i;          /* n + 6 - i  (unsigned addition) */
        seq_t term = binom(N, 6);        /* binomial coefficient (n+6-i choose 6) */
        result += (seq_t)coeff[i] * term;/* multiply by small coefficient and add */
    }
    return result;
}

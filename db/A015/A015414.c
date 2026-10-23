/*
 * A015414  Gaussian binomial coefficient [ n,11 ] for q=-9.
 * https://oeis.org/A015414
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{i=1..11} ((-9)^(n-i+1)-1)/((-9)^i-1)
 * Range:     i8 n <= 11, i16 n <= 11, i32 n <= 11, i64 n <= 12, i128 n <= 14,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    const int K = 11;                     /* width of the Gaussian coefficient */
    if (n < (idx_t)K)                      /* below the offset – value irrelevant */
        return 0;

    seq_t comb[K + 1];
    for (int i = 0; i <= K; ++i)
        comb[i] = 0;
    comb[0] = 1;                           /* C(0,0) = 1 */

    const seq_t q = -9;                     /* the “q” value */

    for (idx_t curr = 1; curr <= n; ++curr) {
        int max_i = (curr < (idx_t)K) ? (int)curr : K;
        idx_t e_start = curr - (idx_t)max_i;   /* exponent for i = max_i */

        seq_t pow_cur;
        if (e_start == 0) {
            pow_cur = 1;                    /* (-9)^0 = 1 */
        } else {
            pow_cur = seq_pow(q, e_start);
        }

        for (int i = max_i; i >= 1; --i) {
            if (comb[i - 1] != 0) {
                /* C(curr,i) = C(curr-1,i) + (-9)^(curr-i) * C(curr-1,i-1) */
                comb[i] = comb[i] + pow_cur * comb[i - 1];
            }
            pow_cur = pow_cur * q;          /* next exponent: +1 (multiply by -9) */
        }
    }

    return comb[K];
}

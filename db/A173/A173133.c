/*
 * A173133  a(n) = Sinh[(2n-1) ArcSinh[n]].
 * https://oeis.org/A173133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + sqrt(1 + n^2))^(2n-1) + (n - sqrt(1 + n^2))^(2n-1)
 *            all divided by 2 (expanded as a sum over even binomial powers)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    const seq_t s = 1 + n_seq * n_seq;     /* 1 + n^2  */
    const idx_t k = 2 * n - 1;             /* exponent 2n-1 */

    seq_t term_n = seq_pow(n_seq, k);      /* n^(2n-1) */
    seq_t term_s = 1;                      /* (1+n^2)^m  */
    seq_t bin     = 1;                     /* C(k,0)  */
    seq_t result  = 0;

    for (idx_t m = 0; m < n; ++m) {
        /* term = bin * term_n * term_s  */
        seq_t term = bin * term_n;
        term = term * term_s;
        result += term;

        if (m + 1 < n) {
            /* update binomial coefficient to C(k,2(m+1)) */
            idx_t j   = 2 * m;                     /* current even index */
            seq_t num = (seq_t)(k - j) * (seq_t)(k - j - 1);
            seq_t den = (seq_t)(j + 1) * (seq_t)(j + 2);
            bin = (bin * num) / den;

            /* divide term_n by n^2  */
            seq_t denom_n2 = n_seq * n_seq;
            term_n = term_n / denom_n2;

            /* multiply term_s by s  */
            term_s = term_s * s;
        }
    }
    return result;
}

/*
 * A119309  a(n) = binomial(2*n,n) * 6^n.
 * https://oeis.org/A119309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n,n) * 6^n Recurrence: a(0)=1,
 *            a(n)=a(n-1)*12*(2*n-1)/n (for n>0)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(n + i);   /* numerator factor n+i */
        binom = (binom * num) / i;    /* binomial coefficient C(2n,n) */
    }
    seq_t pow6 = big_pow(6, (seq_t)n);  /* 6^n  */
    return binom * pow6;
}

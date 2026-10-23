/*
 * A166231  a(n) = 2^C(n+1,2)*A006012(n).
 * https://oeis.org/A166231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(C(n+1,2)) * Sum_{k=0..⌊n/2⌋} C(n,2k) * 2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = 2^(n(n+1)/2) * S(n)  where S(n) = sum_{k} C(n,2k)*2^(n-k) */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* sum S(n):  Σ_{k=0..⌊n/2⌋} binom(n,2k) * 2^(n-k) */
    seq_t sum = 0;
    seq_t binom = 1;                     /* binom(n,0) */
    seq_t pow2n = pow2(n);                /* 2^n   */
    idx_t m = 0;                          /* current even m = 2k */

    for (idx_t k = 0; k <= n / 2; ++k)
    {
        /* 2^(n-k)  = 2^n >> k  */
        seq_t pow_nk = pow2n >> (unsigned int)k;
        sum += binom * pow_nk;

        /* compute next binom(n, m+2) when possible */
        if ((idx_t)(m + 2) > n) break;

        seq_t a1 = (seq_t)(n - m);
        seq_t a2 = (seq_t)(n - m - 1);
        seq_t num = binom * a1 * a2;
        seq_t denom = ((seq_t)(m + 1)) * (m + 2);
        binom = num / denom;

        m += 2;
    }

    /* factor 2^(C(n+1,2)) = 2^(n(n+1)/2) */
    seq_t exp = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t pow2C = (seq_t)1;
    pow2C <<= (unsigned int)exp;

    return pow2C * sum;
}

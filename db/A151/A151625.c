/*
 * A151625  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 3 adjacent element pairs in decreasing order.
 * https://oeis.org/A151625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - (2*n + 1)*6^n + binomial(2*n+1, 2)*3^n -
 *            binomial(2*n+1, 3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 10^n */
    seq_t pow10_n = pow10(n);

    /* 6^n and 3^n */
    seq_t pow6_n  = seq_pow((seq_t)6, n);
    seq_t pow3_n  = seq_pow((seq_t)3, n);

    /* Compute binomial coefficients using 64‑bit arithmetic */
    idx_t two_n   = 2 * n;          /* 2*n */
    idx_t two_n1  = two_n + 1;      /* 2*n+1 */

    idx_t binom2_int = two_n1 * two_n / 2;                     /* C(2n+1,2) */
    idx_t binom3_int = (two_n1 * two_n * (two_n - 1)) / 6;    /* C(2n+1,3) */

    seq_t binom2 = (seq_t)binom2_int;
    seq_t binom3 = (seq_t)binom3_int;

    /* Compute (2n+1)*6^n and C(2n+1,2)*3^n */
    seq_t termA = pow6_n * (seq_t)two_n1;   /* (2n+1) * 6^n */
    seq_t termB = pow3_n * binom2;          /* C(2n+1,2) * 3^n */

    /* C = (2n+1)*6^n - C(2n+1,2)*3^n + C(2n+1,3) */
    seq_t temp = termA - termB;             /* safe: termA >= termB */
    temp = temp + binom3;                   /* safe addition */

    /* Final result: 10^n - C */
    seq_t result = pow10_n - temp;
    return result;
}

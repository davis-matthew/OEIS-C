/*
 * A151647  Number of permutations of 5 indistinguishable copies of 1..n with
 *          exactly 2 adjacent element pairs in decreasing order.
 * https://oeis.org/A151647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^n - (5*n + 1)*6^n + 5*n*(5*n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                     /* n as seq_t            */
    seq_t term1 = big_pow((seq_t)21, sn);    /* 21^n                  */
    seq_t pow6  = big_pow((seq_t)6, sn);     /* 6^n                   */
    seq_t f = (seq_t)5 * sn + 1;             /* 5*n + 1               */
    seq_t term2 = f * pow6;                  /* (5*n+1)*6^n           */
    seq_t t3 = (seq_t)5 * sn * f;            /* 5*n*(5*n+1)           */
    t3 = t3 / 2;                             /* divide by 2            */
    seq_t res = term1 - term2 + t3;          /* final result           */
    return res;
}

/*
 * A115389  Denominator of rational part of raw moment n of the line point
 *          picking problem.
 * https://oeis.org/A115389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 1 + (2*n)*Sum_{k = 1..n} (-1)^(n+k+1)/k
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sum S(n) = Σ_{k=1..n} (-1)^(n+k+1)/k */
    seq_t num = 0;      /* absolute value of numerator */
    seq_t den = 1;      /* denominator, always positive */
    int sign = 0;       /* 0 = zero, +1 = positive, -1 = negative */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* sign of the k-th term */
        int term_sign = ((n + k + 1) & 1) ? -1 : 1;   /* (-1)^(n+k+1) */

        if (sign == 0)                    /* sum was zero */
        {
            sign = term_sign;
            num = 1;                      /* numerator is 1 */
            den = k;                      /* denominator is k */
            continue;
        }

        /* Add term_sign / k to sign/num/den */
        seq_t pk = num * k;               /* num * k  (positive) */

        seq_t new_num_inc = pk;           /* corresponds to sign*num/k   */
        seq_t new_num_term = den;         /* corresponds to 1/k (denote term numerator) */

        int new_sign;
        seq_t new_num;

        if (sign == term_sign)            /* same sign: add magnitudes */
        {
            new_sign = sign;
            new_num = new_num_inc + new_num_term;
        }
        else if (new_num_inc > new_num_term)  /* |sign*num/k| > |1/k| */
        {
            new_sign = sign;
            new_num = new_num_inc - new_num_term;
        }
        else if (new_num_inc < new_num_term)  /* |1/k| > |sign*num/k| */
        {
            new_sign = term_sign;
            new_num = new_num_term - new_num_inc;
        }
        else                                /* magnitudes equal -> zero */
        {
            new_sign = 0;
            new_num = 0;
        }

        seq_t new_den = den * k;           /* denominator after addition */
        seq_t g = gcd(new_num, new_den);

        if (g != 0)
        {
            new_num /= g;
            new_den /= g;
        }
        else
        {
            new_den = 1;                   /* only possible if new_num==0 */
        }

        sign = new_sign;
        num  = new_num;
        den  = new_den;
    }

    /* Compute final fraction: 1 + 2*n*S(n) = (den + sign*(2*n*num)) / den */
    seq_t two_n = (seq_t)n * 2;
    seq_t product = two_n * num;          /* |2*n*num|, always non‑negative */

    seq_t abs_num;
    if (sign == -1)
    {
        if (product > den)
            abs_num = product - den;
        else if (product < den)
            abs_num = den - product;
        else
            abs_num = 0;
    }
    else if (sign == 1)
    {
        abs_num = product + den;
    }
    else /* sign == 0 */
    {
        abs_num = den;
    }

    seq_t g_final = gcd(abs_num, den);
    if (g_final == 0) /* should not happen, but protects divide by zero */
        return den;
    return den / g_final;
}

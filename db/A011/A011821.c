/*
 * A011821  Number of M-sequences m_0,...,m_5 with m_1 < n.
 * https://oeis.org/A011821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 *  OEIS A011821
 *  Number of M‑sequences m_0,…,m_5 with m_1 < n
 *
 *  Formula:
 *      a(n) = Σ_{m=1}^{16} c_m · binom(n‑m+15 , 15)
 *
 *  where the integer coefficients c_m are
 *
 *      c = {  0,   2, -25,  192, -427, 4110, 3582, 21034, 2775,
 *                 12080, -5776, 3712, -1648,   532,  -118,   16,
 *                -1 }
 *
 *********************************************************************/

#include <stdlib.h>

/*----------------------------------------------------------------------------
 *  binom_15(k)  --  Binomial coefficient C(k,15)  for k ≥ 15
 *
 *  The computation uses the multiplicative recurrence
 *
 *      C(k,15) = C(k,14) * (k-14) / 15
 *
 *  to keep intermediate values small and to guarantee exact division.
 *  For built‑in 8‑byte integer types the product is carried out in
 *  unsigned __int128 when the compiler supports it; otherwise the
 *  generic multiplication/division (which works for big‑integer
 *  types as well) is used.
 *--------------------------------------------------------------------------*/
static inline seq_t binom_15(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= 15; ++i)
    {
        idx_t num = k - 15 + i;            /* n‑k+1 in the recurrence */
#ifdef __SIZEOF_INT128__
        /* Use 128‑bit arithmetic only when seq_t is a built‑in type ≤64bit */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 prod = (unsigned __int128)r * (unsigned __int128)num;
            r = (seq_t)(prod / i);
        }
        else
#endif
        {
            r = r * (seq_t)num / i;
        }
    }
    return r;
}

/* Signed coefficients c_m, indexed from 1 (c[0] unused) */
static const int coeff[17] =
{
    0,
    2,   -25, 192, -427, 4110, 3582, 21034, 2775,
    12080, -5776, 3712, -1648, 532, -118, 16, -1
};

/*--------------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The sum of positive and negative contributions */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    /* For m > n the binomial term is zero, so we can stop early */
    for (idx_t m = 1; m <= 16; ++m)
    {
        if (n < m) break;

        /* k = n‑m+15  (k ≥ 15 for n ≥ 1 and m ≤ 16) */
        seq_t bin = binom_15(n - m + 15);

        int c = coeff[(int)m];
        seq_t term = bin * (seq_t)(c >= 0 ? (unsigned int)c : (unsigned int)(-c));

        if (c > 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* All terms are non‑negative and sum_pos ≥ sum_neg */
    return sum_pos - sum_neg;
}

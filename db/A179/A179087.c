/*
 * A179087  Triangle T(n,k) read by rows: the real part of the coefficient
 *          [x^k] of (1-x)^(n+1) * Sum_{s>=0} ((2*s + 1 + 2*i)^n)*x^s, where i
 *          is the imaginary unit.
 * https://oeis.org/A179087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where r is the row containing term n * (r =
 *            floor((sqrt(8n+1)-1)/2) and c = n - r*(r+1)/2). * T(r,c) is the
 *            real part of the coefficient [x^c] of * (1-x)^(r+1) * Sum_{s>=0}
 *            ((2s+1+2i)^r) * x^s. * The triangle T is read row‑wise to
 *            produce the sequence a(0),a(1),
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 59, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   real_of_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/real_of_power.h"

/* Find the row r (0‑based) containing the n‑th element of the triangle. */
static inline idx_t find_row(idx_t n)
{
    idx_t r = 0;      /* current row index */
    idx_t tri = 0;    /* sum of lengths of rows up to and including r-1 */
    while (tri + (r + 1) <= n)
    {
        ++r;
        tri += r;
    }
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Determine row r and column c within that row. */
    idx_t r = find_row(n);
    idx_t tri = r * (r + 1) / 2;
    idx_t c = n - tri;           /* 0 ≤ c ≤ r */

    /* Compute T(r,c) = sum_{j=0}^{c} (-1)^j binom(r+1,j) Re((2(c-j)+1+2i)^r). */
    seq_t sum = 0;
    seq_t binom = 1;             /* binom(r+1,0) */
    idx_t r_plus_one = r + 1;

    for (idx_t j = 0; j <= c; ++j)
    {
        seq_t a_val = 2 * (c - j) + 1;
        seq_t real_val = real_of_power(a_val, r);
        seq_t term = binom * real_val;

        if ((j & 1) == 0)
            sum += term;         /* add for even j */
        else
            sum -= term;         /* subtract for odd j */

        /* Update binomial for next j; only if we are not at the last iteration. */
        if (j < c)
            binom = binom * (seq_t)(r_plus_one - j) / (seq_t)(j + 1);
    }

    return sum;
}

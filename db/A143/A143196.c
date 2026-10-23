/*
 * A143196  Triangle read by rows: real part of Lerch Phi expansion of p(x,n)
 *          = 2^n*(1 - i*x)^(n+1) * LerchPhi(i*x, -n, 1/2).
 * https://oeis.org/A143196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = real part of the coefficient of x^c in p(x, r) = 2^r (1 -
 *            i x)^(r+1) LerchPhi(i x, -r, 1/2), where r and c are the row and
 *            column corresponding to the flattened index n: r =
 *            ⌊(√(1+8n)-1)/2⌋, c = n - r(r+1)/2. The real part is non‑zero
 *            only for even columns: let m = c/2, then a(n) = (-1)^m...
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 69, i64 n <= 160,
 *            i128 n <= 446, big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/pow_seq.h" /* seq_t pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t r;
    {
        double d = (sqrt(1.0 + 8.0 * (long double)n) - 1.0) / 2.0;
        r = (idx_t)floor(d);
        /* Correct any round‑off error. */
        while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
        while ((unsigned __int128)r * (r + 1) / 2 > n)    --r;
    }

    /* Column c in the row.                            */
    idx_t c = n - (unsigned __int128)r * (r + 1) / 2;

    /* Odd columns give zero.                          */
    if (c & 1U) return (seq_t)0;

    /* Summation part of the formula.                  */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= c; ++k) {
        seq_t bin = binom(r + 1, k);
        if (k & 1U) bin = -bin;
        seq_t base = (seq_t)(2 * c + 1 - 2 * k);    /* 2c+1-2k > 0    */
        seq_t p    = pow_seq(base, (int)r);        /* (2c+1-2k)^r     */
        sum += bin * p;
    }

    /* Apply the sign factor (-1)^(c/2).                */
    idx_t half = c >> 1U;                          /* c/2            */
    seq_t sign = (half & 1U) ? (seq_t)(-1) : (seq_t)1;
    return sign * sum;
}

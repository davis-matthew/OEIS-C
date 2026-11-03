/*
 * A113287  Triangle T, read by rows, where row n of T equals row n of matrix
 *          (n+1)-th power of triangle A112555.
 * https://oeis.org/A113287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of x^r y^c in A(x,y) = 1/(1-xy) +
 *            x*(x+2)/((1-xy)^2*(1+x+xy)^2), where r = row_of_index(n) and c =
 *            n - r(r+1)/2. The coefficient is computed by a closed‑form
 *            summation derived from the generating function. Only integer
 *            arithmetic is used
 * Range:     i8 n <= 37, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

static seq_t w_value(idx_t p, idx_t c)
{
    /* Returns w_{p,c} = sum_{i=0}^{min(p,c)} (i+1)(p-i+1)(-1)^{p-i} * binom(p-i, c-i) */
    if (c > p) return 0;
    seq_t sum = 0;
    for (idx_t i = 0; i <= (p < c ? p : c); ++i) {
        seq_t bin = binomial(p - i, c - i);
        seq_t term = bin;
        term *= (seq_t)(i + 1);
        term *= (seq_t)(p - i + 1);
        if (((p - i) & 1) != 0) term = -term;
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);                /* row number */
    idx_t c = n - r * (r + 1) / 2;            /* column number */
    if (r == c) return (seq_t)1;              /* diagonal always 1 */

    seq_t w1 = 0, w2 = 0;
    if (r >= 2) w1 = w_value(r - 2, c);
    if (r >= 1) w2 = w_value(r - 1, c);

    seq_t G = w1 + (seq_t)2 * w2;             /* G_{r,c} */
    return G;                                 /* no diagonal contribution for r != c */
}

/*
 * A230210  Trapezoid of dot products of row 7 (signs alternating) with
 *          sequential 8-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 8-tuples (C(7,0), -C(7,1),
 *          ..., C(7,6), -C(7,7)) and (C(n-1,k-7), C(n-1,k-6), ..., C(n-1,k)),
 *          n >= 1, 0 <= k <= n+6.
 * https://oeis.org/A230210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) is the first component of the trapezoid T(n,k) * with *
 *            T(n,k) = Σ_{i=0..7} (-1)^(i+7) * C(7,i) * C(n-1, k-i), * where
 *            C(x,y)=0 when y<0 or y>x. * The sequence a is obtained by
 *            listing the rows of T * from left to right, top to bottom
 * Range:     i8 n <= 150, i16 n <= 419, i32 n <= 814, i64 n <= 1625,
 *            i128 n <= 1625, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"  /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* find the row r such that the first r rows contain at least n terms */
    idx_t cumulative = 0;           /* number of terms seen so far */
    idx_t r = 1;                    /* current row number (1‑based) */
    while (1) {
        cumulative += r + 7;        /* number of terms in row r */
        if (cumulative >= n)
            break;                  /* row r contains the n‑th term */
        ++r;
    }

    /* index of the term within row r (0‑based) */
    idx_t before = cumulative - (r + 7);   /* terms before row r */
    idx_t k = n - before - 1;               /* column index k = c */

    /* compute T(r,k) */
    const int coeffs[8] = {1, 7, 21, 35, 35, 21, 7, 1}; /* C(7,i) */
    seq_t result = 0;
    idx_t n_minus_1 = r - 1;                /* n-1 in the formula */

    for (int i = 0; i < 8; ++i) {
        if (k < (idx_t)i)          /* k-i < 0  => binomial = 0 */
            continue;
        idx_t t = k - i;           /* k-i */
        if (t > n_minus_1)         /* binomial 0 for t > n-1 */
            continue;
        seq_t term = binomial(n_minus_1, t);   /* C(n-1, k-i) */
        seq_t coeff = coeffs[i];
        /* sign: (-1)^(i+7)  => negative when (i+7) is odd */
        if ((i + 7) & 1)          /* odd -> negative sign */
            result -= coeff * term;
        else                     /* even -> positive sign */
            result += coeff * term;
    }
    return result;
}

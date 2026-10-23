/*
 * A156786  The triangular sequence of symmetrical Lah numbers (A111596,
 *          A008297) : L(n, m) = (-1)^n* binomial(n,k)*binomial(n-1, k-1)*(
 *          (n-k)! + (n-k)*(k-1)! ), with L(0,0) = 2, L(n,0) = L(n,n) =
 *          (-1)^n.
 * https://oeis.org/A156786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the triangular sequence of symmetrical Lah numbers *
 *            L(r,m) = (-1)^r * C(r,m) * C(r-1,m-1) * * ((r-m)! +
 *            (r-m)*(m-1)!) * with the convention L(0,0)=2 and
 *            L(r,0)=L(r,r)=(-1)^r
 * Range:     i8 n <= 15, i16 n <= 36, i32 n <= 79, i64 n <= 211,
 *            i128 n <= 563, big any n
 * Helpers:   factorial_simple, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial_simple.h"
#include "../utils/offset_row.h"

/* multiplicative binomial coefficient: C(n,k) */
static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;      /* not needed, safety */
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

/* offset of the first element of row r: r*(r+1)/2 */

/* OEIS sequence A156786 */
seq_t a(idx_t n)
{
    /* Find row r such that offset_row(r) <= n < offset_row(r+1) */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        ++r;

    idx_t m = n - offset_row(r);   /* column index within the row */

    /* Special cases */
    if (r == 0 && m == 0)
        return (seq_t)2;

    if (m == 0 || m == r) {
        /* (-1)^r */
        return (seq_t)((r & 1) ? -1 : 1);
    }

    /* Compute the terms of the formula */
    seq_t sign = (seq_t)((r & 1) ? -1 : 1);
    seq_t C1   = binom_multiplicative(r, m);
    seq_t C2   = binom_multiplicative(r - 1, m - 1);
    seq_t fact_n_m = factorial_simple(r - m);
    seq_t fact_m_1 = factorial_simple(m - 1);
    seq_t term3 = fact_n_m + (seq_t)(r - m) * fact_m_1;

    seq_t result = sign;
    result *= C1;
    result *= C2;
    result *= term3;
    return result;
}

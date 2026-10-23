/*
 * A173567  Triangle T(n, k) = (f(k, n-k+1) + f(n-k+1, k))/2 where f(n, k) =
 *          (1/2)*Sum_{j=1..2*n} k^j, read by rows.
 * https://oeis.org/A173567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = (f(k,n-k+1)+f(n-k+1,k))/2, f(m,t) =
 *            t*(1 - t^(2m))/(1-t) for t≠1, f(m,1) = 2m. The triangle is read
 *            by rows
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 68, u64 n <= 157,
 *            u128 n <= 397, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"   /* for pow_seq(seq_t base, idx_t exp) */

/* helper: f(m,t) with m in idx_t, t in seq_t */
static seq_t f_m_t(idx_t m, seq_t t)
{
    if (t == (seq_t)1U)          /* t = 1 gives sum of 1^j = 2m */
        return (seq_t)(2U * m);

    /* compute t^(2m) */
    seq_t pow = pow_seq(t, (idx_t)(2U * m));

    /* numerator: t * (pow - 1) */
    seq_t num = t * (pow - (seq_t)1U);

    /* denominator: t - 1 */
    seq_t den = t - (seq_t)1U;

    /* exact division */
    return num / den;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 ≥ n  (row r has r entries) */
    idx_t r = 1U;
    while ( (r * (r + 1U)) / 2U < n )
        ++r;

    /* previous total elements before this row: (r-1)*r/2 */
    idx_t prev = (r - 1U) * r / 2U;
    idx_t k = n - prev;          /* 1‑based column index in row r */

    /* T(r,k) = (f(k,r-k+1)+f(r-k+1,k))/2 */
    seq_t f1 = f_m_t(k, (seq_t)(r - k + 1U));
    seq_t f2 = f_m_t(r - k + 1U, (seq_t)k);
    return (f1 + f2) / 2U;
}

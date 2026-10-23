/*
 * A187105  Triangle T(n,k) read by rows: number of height-2-restricted finite
 *          functions.
 * https://oeis.org/A187105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, tabl
 * Formula:   a(n) is read by rows from the triangle T where T(n,k) =
 *            Σ_{j=0}^{n+1-k} binomial(n+1-k,j) * k^j * j^(n+1-k-j) For n ≥ 1,
 *            find the smallest r such that (r+1)(r+2)/2 ≥ n, let k = n –
 *            r(r+1)/2, and return T(r,k)
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & (idx_t)1)
            res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

static seq_t triangle_value(idx_t row, idx_t k)
{
    /* m = row + 1 – k is the size of the domain */
    idx_t m = row + 1 - k;
    seq_t sum = 0;
    for (idx_t j = 0; j <= m; ++j)
    {
        seq_t term = binom(m, j);                  /* C(m,j)   */
        term = term * pow_int((seq_t)k, j);        /* * k^j    */
        term = term * pow_int((seq_t)j, m - j);    /* * j^(m-j) */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                /* offset is 1 */

    /* Find the row r such that the cumulative length
       up to that row is at least n */
    idx_t row = 0;
    idx_t cumulative = 1;                         /* length of row 0 */
    while (cumulative < n)
    {
        ++row;
        cumulative += row + 1;                    /* add length of next row */
    }

    /* Column index k (1‑based) within this row */
    idx_t prev_cumulative = cumulative - (row + 1);
    idx_t k = n - prev_cumulative;

    return triangle_value(row, k);
}

/*
 * A164866  (101^n,1)-Pascal triangle.
 * https://oeis.org/A164866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = entry (r,c) of the (101^n,1)-Pascal triangle. Rows are
 *            numbered starting at 1. Row r contains r numbers generated as
 *            follows: x1 = 1 for i = 1 .. r-1: xi+1 = xi * 100 + C(r-1,i)
 *            Then the sequence a(n) lists the rows one after another
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t binom(idx_t n, idx_t k)
{
    seq_t res = (seq_t)1;
    for (idx_t j = 1; j <= k; ++j)
    {
        res = res * (seq_t)(n - j + 1) / (seq_t)j;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    idx_t t = (idx_t)1;          /* t = r*(r+1)/2 */
    while (t < n)
    {
        ++r;
        t = r * (r + 1) / 2;
    }

    idx_t prev_tri = (r - 1) * r / 2;
    idx_t offset   = n - prev_tri - 1;  /* 0‑based position in the row */

    if (offset == 0)   /* first element of any row is 1 */
        return (seq_t)1;

    seq_t val = (seq_t)1;
    idx_t k_row = r - 1;   /* the top of the binomial row */

    for (idx_t i = 1; i <= offset; ++i)
    {
        seq_t c = binom(k_row, i);   /* C(k_row, i) */
        val = val * (seq_t)100 + c;
    }

    return val;
}

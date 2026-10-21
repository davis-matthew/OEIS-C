/*
 * A204204  Triangle based on (0,3/4,1) averaging array.
 * https://oeis.org/A204204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where the triangle T is defined by * T(1,1) = 3;
 *            T(2,1)=3; T(2,2)=7; * T(n,k) = T(n-1,k) + 3*T(n-1,k-1) -
 *            2*T(n-2,k-1) - 2*T(n-2,k-2), * with T(n,k)=0 for k<1 or k>n. *
 *            The sequence a(n) is the triangle read row‑by‑row
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t triangle_entry(idx_t r, idx_t c)
{
    /* Special case column 1 – always 3. */
    if (c == 1)
        return (seq_t)3;

    /* m = r-1, because binomials are taken with index m = r-1. */
    idx_t m = r - 1;
    seq_t sum = (seq_t)0;      /* ∑_{j=0}^{c-2} binom(m,j)          */
    seq_t binom = (seq_t)1;    /* binom(m,0)                         */

    /* Sum binomials up to j = c-2. */
    idx_t j;
    for (j = 0; j < c - 1; ++j) {
        if (j > 0) /* update binom to binom(m,j) from binom(m,j-1) */
            binom = binom * ((seq_t)(m - j + 1)) / (seq_t)j;
        sum += binom;
    }

    /* Compute binom(m, c-1). After the loop binom == binom(m, c-2). */
    if (c > 1)
        binom = binom * ((seq_t)(m - (c - 1) + 1)) / (seq_t)(c - 1);

    /* Final formula: 4 * sum + 3 * binom(m, c-1).               */
    return (seq_t)4 * sum + (seq_t)3 * binom;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but safe */

    /* Find the row r such that r(r+1)/2 ≥ n.               */
    idx_t r = 1;
    idx_t cumulative = 1;                  /* i.e. 1*2/2          */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column within that row.                            */
    idx_t prev_cumulative = cumulative - r; /* (r-1)r/2          */
    idx_t c = n - prev_cumulative;          /* 1‑based column     */

    return triangle_entry(r, c);
}

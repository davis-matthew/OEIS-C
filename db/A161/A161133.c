/*
 * A161133  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having exactly k odd fixed points (0 <= k <=
 *          ceiling(n/2)).
 * https://oeis.org/A161133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 62, u64 n <= 130,
 *            u128 n <= 340, big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161133: Triangle read by rows
   T(n,k) = C(ceil(n/2),k) * Σ_{j=0}^{ceil(n/2)-k} (-1)^j * (n-k-j)! * C(ceil(n/2)-k,j)
   the sequence a(n) is the flattened triangle (row by row). */
#include "../utils/factorial.h"

static seq_t choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* external linkage as required */
seq_t a(idx_t n)
{
    /* locate the row r and column k of index n in the flattened triangle */
    idx_t r = 0;
    idx_t pos = n;                      /* remaining offset within rows */
    while (1)
    {
        /* length of row r: ceil(r/2)+1 = ((r+1)/2)+1 */
        idx_t len = ((r + 1) / 2) + 1;
        if (pos < len) break;
        pos -= len;
        ++r;
    }
    idx_t k = pos;                       /* column index in row r */

    idx_t m = (r + 1) / 2;                /* ceil(r/2) */

    seq_t binom_row = choose(m, k);       /* C(m,k) */
    if (binom_row == 0) return (seq_t)0;

    idx_t maxj = m - k;                   /* upper bound of j in the sum */

    /* factorial of (r - k) once, subsequently divided by decreasing integers */
    seq_t fact = factorial(r - k);
    seq_t sum  = 0;

    for (idx_t j = 0; j <= maxj; ++j)
    {
        seq_t binom_rest = choose(maxj, j);          /* C(m-k, j) */
        seq_t term = fact * binom_rest;
        if ((j & 1) == 0)
            sum += term;
        else
            sum -= term;

        /* prepare factorial for next j: fact = (r-k-(j+1))! */
        if (j < maxj)
        {
            seq_t denom = (seq_t)(r - k - j);
            fact = fact / denom;
        }
    }

    return binom_row * sum;
}

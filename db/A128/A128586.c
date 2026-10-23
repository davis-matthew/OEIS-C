/*
 * A128586  Triangle read by rows: A007318^(-1) * A128540.
 * https://oeis.org/A128586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = binom(r,j)*(-1)^(r-j)*Fib(j+1) +
 *            binom(r,j+1)*(-1)^(r-(j+1))*Fib(j+2) where r and j are the
 *            0‑based row and column indices of the term a(n) in the triangle
 *            (row 0 has 1 term, row 1 has 2 terms, etc.)
 * Range:     i8 n <= 39, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A128586 (1‑based index).  All arithmetic
   is performed in seq_t; intermediate values are kept as small as
   possible.  Auxiliary functions are defined as static helpers
   inside this file. */

#include <stddef.h>

/* Integer square root of a 128‑bit integer using binary search
   (no floating point).  Returns floor(sqrt(x)). */
static inline idx_t
isqrt_u128 (unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    idx_t            res   = 0;

    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;

        if (sq <= x)
        {
            res = (idx_t)mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return res;
}

/* Compute binomial coefficient C(n,k) using the multiplicative
   formula.  The calculation is exact for integer arithmetic. */
static inline seq_t
binom_idx (idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Fibonacci number F(k), where F(1)=1, F(2)=1.  Iterative, small k. */
static inline seq_t
fib (idx_t k)
{
    if (k == 1 || k == 2)
        return 1;
    seq_t a = 1; /* F1 */
    seq_t b = 1; /* F2 */
    seq_t c;
    for (idx_t i = 3; i <= k; ++i)
    {
        c = a + b;   /* next Fibonacci number */
        a = b;
        b = c;
    }
    return b;
}

seq_t
a (idx_t n)
{
    if (n == 0)
        return 0;   /* not used, but guard against n=0 */

    /* 0‑based index of the term in the flattened triangle */
    idx_t m = n - 1;

    /* Find the row r using the triangular number inverse:
       r = floor((sqrt(8*m + 1) - 1) / 2). */
    unsigned __int128 val = (unsigned __int128)8 * m + 1;
    idx_t           t   = isqrt_u128 (val);
    idx_t           r   = (t - 1) >> 1;   /* (t-1)/2 */

    /* Index of the first term of this row */
    idx_t start = (r * (r + 1)) >> 1;     /* r*(r+1)/2 */

    idx_t j = m - start;                  /* column index (0‑based) */

    /* First contribution: from column j */
    seq_t term1 = binom_idx (r, j) * fib (j + 1);
    if (((r - j) & 1) != 0)
        term1 = -term1;

    /* Second contribution: from column j+1 (if it exists) */
    seq_t term2 = 0;
    if (j + 1 <= r)
    {
        term2 = binom_idx (r, j + 1) * fib (j + 2);
        if (((r - (j + 1)) & 1) != 0)
            term2 = -term2;
    }

    return term1 + term2;
}

/*
 * A143255  Triangle read by rows, A128407 * A126988; 1<=k<=n.
 * https://oeis.org/A143255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   A143255 is read by rows from the triangle * t(n,m)=μ(n)*f(n,m)
 *            where * f(n,m)=n for m==1 * f(n,m)=n/m if m divides n * f(n,m)=0
 *            otherwise. * The sequence is a(T(n-1)+m)=t(n,m) with
 *            T(n)=n(n+1)/2
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute the Möbius function μ(n) for n ≥ 1
 * μ(n)=1 if n has an even number of distinct prime factors,
 * μ(n)=-1 if odd, μ(n)=0 if a squared prime divides n. */
static seq_t mobius(idx_t n)
{
    if (n == 0) return 0;      /* safety, μ(0) is undefined but will not be used */
    int distinct = 0;          /* number of distinct prime factors */
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                ++exp;
                tmp /= p;
                if (exp > 1)          /* squared factor → μ(n)=0 */
                    return 0;
            }
            ++distinct;
        }
    }
    if (tmp > 1) ++distinct;    /* remaining prime factor */
    return (distinct & 1) ? -1 : 1;
}

/* Return triangular number T(n)=n(n+1)/2 as idx_t */
static inline idx_t triangular(idx_t n)
{
    return n * (n + 1) / 2;
}

/* Return the n-th term of A143255 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, but keep safe */

    /* Find the minimal row r with T(r) ≥ n */
    idx_t low  = 1;
    idx_t high = n;                       /* r can never exceed n */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;                       /* row index (1‑based) */

    idx_t prev_tri = triangular(row - 1);  /* T(row-1) */
    idx_t col = n - prev_tri;              /* column index within the row (1‑based) */

    seq_t mu = mobius(row);
    seq_t result;

    if (col == 1)
    {
        result = mu * (seq_t)row;           /* μ(row)·row */
    }
    else if (row % col == 0)
    {
        result = mu * (seq_t)(row / col);   /* μ(row)·(row/col) */
    }
    else
    {
        result = 0;
    }

    return result;
}

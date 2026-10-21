/*
 * A092565  Triangle of coefficients T(n,k) (n>=0, 0<=k<=2*n), read by rows,
 *          where the n-th row polynomial equals the numerator of the n-th
 *          convergent of the continued fraction [1+x+x^2;1+x+x^2,1+x+x^2,...]
 *          for n>0, with the zeroth row defined as T(0,0)=1.
 * https://oeis.org/A092565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 149, u32 n <= 454, u64 n <= 1626,
 *            u128 n <= 5988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A092565: Triangle coefficient T(n,k) for linear index n.
   The sequence is read row‑wise from the triangle T where
   T(n,k) equals the number of lattice paths from (0,0) to (n,k)
   using steps (1,0),(2,0),(1,1),(1,2).  For a linear index n
   we find the row r = ⌊√n⌋ and column k = n - r², then compute
   T(r,k) by simple dynamic programming.  All arithmetic is done
   in the build's seq_t type so it works for 8/16/32/64 bit and
   arbitrary‑precision integers. */
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/*  Compute ⌊√x⌋ for unsigned 64‑bit values.  The estimation by
    sqrt((double)x) is corrected with 128‑bit integer arithmetic.  */
static idx_t isqrt_idx(idx_t x)
{
    if (x < 2)
        return (idx_t)x;
    idx_t r = (idx_t)sqrt((double)x);
    while ((unsigned __int128)r * r > x) --r;
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    return r;
}

/*  Compute the triangle entry T(n,k) by dynamic programming.  The
    DP table has (n+1) rows and (2*n+1) columns.  */
static seq_t triangle_T(idx_t n, idx_t k)
{
    if (k > 2 * n)
        return (seq_t)0;

    const idx_t rows = n + 1;
    const idx_t cols = 2 * n + 1;
    const idx_t size = rows * cols;

    /*  Allocate and zero the table.  */
    seq_t *dp = (seq_t *)calloc((size_t)size, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;               /* graceful out‑of‑memory */

    dp[0] = (seq_t)1;   /* dp[0][0] */

    for (idx_t i = 0; i <= n; ++i)
    {
        for (idx_t j = 0; j <= 2 * n; ++j)
        {
            seq_t cur = dp[i * cols + j];
            if (cur == (seq_t)0)
                continue;

            if (i + 1 <= n)                /* (1,0) */
                dp[(i + 1) * cols + j] += cur;
            if (i + 2 <= n)                /* (2,0) */
                dp[(i + 2) * cols + j] += cur;
            if (i + 1 <= n && j + 1 <= 2 * n) /* (1,1) */
                dp[(i + 1) * cols + j + 1] += cur;
            if (i + 1 <= n && j + 2 <= 2 * n) /* (1,2) */
                dp[(i + 1) * cols + j + 2] += cur;
        }
    }

    seq_t result = dp[n * cols + k];
    free(dp);
    return result;
}

/*  The public accessor required by the harness.  */
seq_t a(idx_t n)
{
    idx_t r = isqrt_idx(n);          /* row index */
    idx_t k = n - r * r;             /* column index in that row */
    return triangle_T(r, k);
}

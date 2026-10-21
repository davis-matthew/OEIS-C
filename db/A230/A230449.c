/*
 * A230449  T(n, k) = T(n-1, k-1) + T(n-1, k) with T(n, 0) = 1 and T(n, n) =
 *          A052952(n), n >= 0 and 0 <= k <= n.
 * https://oeis.org/A230449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th element of the triangle T, read row‑wise.
 *            T(n,0) = 1 T(n,n) = F(n+2) - (1-(-1)^n)/2 T(n,k) = T(n-1,k-1) +
 *            T(n-1,k) (1 <= k <= n-1) F(m) is the Fibonacci number with
 *            F(0)=0,F(1)=1. The sequence a(n) lists the triangle values in
 *            the order T(0,0), T(1,0),T(1,1), T(2,0),T(2,1),T(2,2)...
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Compute the row index r such that the first r rows contain
   strictly less than n+1 terms, i.e. r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t
row_of(idx_t n)
{
    /* Use double for the initial estimate, then correct any
       rounding errors.  For the problem limits this is safe. */
    double d = sqrt((double)8 * n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);

    /* Adjust downwards if we over‑estimated. */
    while ((unsigned __int128)r * (r + 1) / 2 > n)
        --r;
    /* Adjust upwards if we underestimated. */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row r and the column k within that row. */
    idx_t r = row_of(n);
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;
    if (k == 0)          /* first element of the row is always 1 */
        return (seq_t)1;

    /* Precompute Fibonacci numbers up to F(r+2). */
    idx_t fsize = r + 3;                      /* indices 0 … r+2 */
    unsigned __int128 *fib = (unsigned __int128 *)
        malloc((size_t)fsize * sizeof(unsigned __int128));
    fib[0] = (unsigned __int128)0;
    fib[1] = (unsigned __int128)1;
    for (idx_t i = 2; i < fsize; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* Allocate two rows of the triangle; we only keep the current
       and the previous row.  Each row needs at most r+1 entries. */
    size_t sz = (size_t)(r + 1);
    unsigned __int128 *prev = (unsigned __int128 *)calloc(sz, sizeof(unsigned __int128));
    unsigned __int128 *curr = (unsigned __int128 *)calloc(sz, sizeof(unsigned __int128));

    /* Row 0. */
    prev[0] = (unsigned __int128)1;

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (unsigned __int128)1;      /* left edge */
        for (idx_t j = 1; j < i; ++j)
            curr[j] = prev[j - 1] + prev[j];
        /* Diagonal element T(i,i). */
        unsigned __int128 val = fib[i + 2];    /* F(i+2)   */
        if (i & 1)                         /* odd i subtract 1 */
            val -= (unsigned __int128)1;
        curr[i] = val;

        /* Prepare for next iteration. */
        unsigned __int128 *tmp  = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = (seq_t)prev[k];

    /* Free all allocated memory. */
    free(prev);
    free(curr);
    free(fib);
    return result;
}

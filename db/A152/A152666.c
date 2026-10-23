/*
 * A152666  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} having k runs of odd entries (0<=k<=ceiling(n/2)). For
 *          example, the permutation 321756498 has 3 runs of odd entries: 3,
 *          175 and 9.
 * https://oeis.org/A152666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the triangle read row‑wise, where
 *            T(2m,k) = (m!)² * binomial(m+1,k) * binomial(m-1,k-1) T(2m+1,k)
 *            = m! * (m+1)! * binomial(m,k-1) * binomial(m+1,k)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 65, u64 n <= 146,
 *            u128 n <= 347, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

/* Compute the n-th entry of the triangle (row‑wise flattened). */
seq_t a(idx_t n)
{
    /* Find the row r that contains index n.
       The length of row r is floor((r)/2)+1 if r is even,
       ay        floor((r)/2)+2 if r is odd. */
    idx_t r   = 0;
    idx_t pos = 0;                     /* index where current row starts */
    while (1) {
        idx_t len = (r & 1) ? (r/2 + 2) : (r/2 + 1);
        if (pos + len > n)
            break;
        pos += len;
        ++r;
    }
    idx_t k = n - pos;                  /* column in row r (0‑based) */

    /* Row 0 has only one entry T(0,0)=1. */
    if (r == 0)
        return (seq_t)1;

    /* Even rows: n = 2m.  T(2m,k) = (m!)² * binomial(m+1,k) * binomial(m-1,k-1). */
    if ((r & 1) == 0) {
        idx_t m = r / 2;
        if (k == 0) /* binomial(m-1,-1)=0, but avoid unsigned underflow */
            return (seq_t)0;

        seq_t res = factorial(m);
        res *= factorial(m);                       /* (m!)² */
        res *= binomial(m + 1, k);
        res *= binomial(m - 1, k - 1);
        return res;
    }

    /* Odd rows: n = 2m+1.  T(2m+1,k) = m! * (m+1)! * binomial(m,k-1) * binomial(m+1,k). */
    idx_t m = (r - 1) / 2;
    if (k == 0) /* binomial(m,-1)=0, avoid underflow */
        return (seq_t)0;

    seq_t res = factorial(m);
    res *= factorial(m + 1);                   /* m! * (m+1)! */
    res *= binomial(m, k - 1);
    res *= binomial(m + 1, k);
    return res;
}

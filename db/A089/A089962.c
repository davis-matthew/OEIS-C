/*
 * A089962  Triangle, read by rows, that equals the matrix inverse of A071207
 *          when treated as a lower triangular matrix.
 * https://oeis.org/A089962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (-1)^(n-k) * C(n,k) * n * k^(n-k-1) read row‑by‑row from
 *            the triangle T(n,k) with T(0,0)=1 and T(n,n)=1
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

static idx_t row_from_index(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;           /* largest r with r(r+1)/2 <= n */
}

static seq_t compute_T(idx_t n, idx_t k)
{
    /* last element of each row is always 1 */
    if (k == n) return 1;

    /* first element (k==0) */
    if (k == 0) {
        seq_t res = (seq_t)n;                     /* binom(n,0)=1, multiply by n */
        seq_t pw  = (n == 1) ? (seq_t)1 : (seq_t)0;/* 0^(n-1): 1 if n==1 else 0   */
        res = res * pw;                           /* 0 for n>1, 1 for n==1     */
        if ((n & 1) == 1) res = -res;             /* sign factor (-1)^n          */
        return res;
    }

    /* general case: 0 < k < n */
    seq_t res = binomial(n, k);                   /* C(n,k)                       */
    res = res * (seq_t)n;                         /* multiply by n                */
    seq_t pw  = big_pow((seq_t)k, (seq_t)(n - k - 1)); /* k^(n-k-1)   */
    res = res * pw;
    if (((n - k) & 1) == 1) res = -res;          /* apply sign (-1)^(n-k)       */
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;              /* first term of the triangle */
    idx_t r    = row_from_index(n);    /* row number   */
    idx_t start = (r * (r + 1)) / 2;   /* cumulative count before row r */
    idx_t k    = n - start;            /* column within the row      */
    return compute_T(r, k);
}

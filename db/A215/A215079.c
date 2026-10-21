/*
 * A215079  Triangle T(n,k) = k^n * sum(binomial(n,n-k-j),j=0..n-k).
 * https://oeis.org/A215079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 59, u64 n <= 144,
 *            u128 n <= 367, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A215079
 *
 *  A(n) is the linear enumeration (row‑wise) of the triangle
 *
 *          T(n,k) = k^n * Σ_{m=0}^{n-k} binomial(n,m)      (0 ≤ k ≤ n)
 *
 *  where n is the row index and k the column.  The enumeration order is:
 *      T(0,0)  T(1,0) T(1,1)  T(2,0) T(2,1) T(2,2)  T(3,0) …
 *
 *  The routine below computes a(n) for any non‑negative n, using only the
 *  provided utilities (big_pow, binomial).  No hard‑coded table is used.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static inline idx_t _row_of(idx_t n)
{
    idx_t r = 0;
    /* Advance r until the next cumulative size would exceed n. */
    while ((seq_t)(r + 1) * (r + 2) / 2 <= n)
        r++;
    return r;                /* r now satisfies the required bounds */
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);
    idx_t start = r * (r + 1) / 2;   /* index of the first element in row r */
    idx_t k = n - start;             /* column within the row */

    /* Special cases that avoid expensive calculations */
    if (r == 0 && k == 0)            /* T(0,0) = 1 */
        return (seq_t)1;
    if (k == 0)                      /* 0^r * sum = 0 for r>0 */
        return (seq_t)0;

    /* Compute k^r */
    seq_t pow_k_r = big_pow((seq_t)k, (seq_t)r);

    /* Compute Σ_{m=0}^{r-k} binomial(r,m) */
    seq_t sum = 0;
    for (idx_t m = 0; m <= r - k; ++m)
        sum += binomial(r, m);

    return pow_k_r * sum;
}

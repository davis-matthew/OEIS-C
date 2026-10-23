/*
 * A173117  Triangle T(n, k, q) = q*[n=2] + Sum_{j=0..5} q^j*binomial(n-2*j,
 *          k-j)*[n>2*j] with T(n,0) = T(n,n) = 1 for q = 1, read by rows.
 * https://oeis.org/A173117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 178, u32 n <= 643, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A173117  */
/*  Triangle T(n,k) read by rows, with   */
/*      T(n,k,1) = (n==2?1:0) + Σ_{j=0..5} C(n-2j, k-j)   */
/*  (using the convention C(a,b)=0 when b<0 or b>a).  */
/*  The sequence a(n) lists the terms of this triangle in
 *  row‑major order: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1),
 *  a(3)=T(2,0), a(4)=T(2,1), a(5)=T(2,2), a(6)=T(3,0), …   */

#include "../utils/binom.h"
#include <stdbool.h>        /* for bool, if needed */

/* Compute the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
static idx_t
row_of(idx_t n)
{
    idx_t r = 0;
    /* Since n ≤ 10 000, r will never exceed 200; a simple loop is fine. */
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    return r;
}

/* Compute a single triangle entry T(n,k) for q = 1.  */
static seq_t
T_value(idx_t n, idx_t k)
{
    /* Endpoints are always 1. */
    if (k == 0 || k == n)
        return (seq_t)1;

    seq_t val = (seq_t)((n == 2) ? 1 : 0);  /* the optional [n==2] term */

    /* Sum over j = 0 .. min(5, floor((n-1)/2), k). */
    for (idx_t j = 0; j <= 5 && 2 * j < n; ++j)
    {
        if (k < j)
            continue;              /* (k-j) would be negative */

        idx_t nk = n - 2 * j;      /* remaining n for this j */
        idx_t kk = k - j;          /* remaining k for this j */

        if (kk > nk)               /* binomial is zero otherwise */
            continue;

        val += binom(nk, kk);
    }

    return val;
}

/* Public entry point as required by the build system. */
seq_t a(idx_t n)
{
    /* Map the linear index n into row r and column k. */
    idx_t r = row_of(n);                /* row index */
    idx_t k = n - r * (r + 1) / 2;      /* column index */

    return T_value(r, k);
}

/*
 * A122919  Inverse of Riordan array (1/(1+x+x^2),x/(1+x)^2).
 * https://oeis.org/A122919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A122919: inverse of the Riordan array (1/(1+x+x²), x/(1+x)²)
 *  The n‑th term (a(0)=1) is defined by
 *
 *  Let r be the largest integer with r(r+1)/2 ≤ n   (row index)
 *  Let k = n – r(r+1)/2                              (column within the row)
 *
 *  For 0 ≤ ℓ ≤ r:
 *      c(r,ℓ) = (ℓ/r) * binom(2r, r-ℓ)               (with c(r,ℓ)=0 if ℓ>r)
 *
 *  The triangle entry in row r, column k is
 *      S(r,k) = c(r,k) + c(r,k+1) + c(r,k+2)
 *
 *  The flattened sequence is a(n)=S(r,k).
 *
 *  The helper `binom` below computes binomial coefficients
 *  multiplicatively; it works for all provided integer types
 *  (including arbitrary precision classes).  No floating‑point
 *  arithmetic is used.
 */

#include <stdint.h>

/*  multiplicative binomial coefficient  */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/*  seq_t a(idx_t n)  */
seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)0;
    while ( ((r + (idx_t)1) * (r + (idx_t)2)) / (idx_t)2 <= n )
        ++r;

    idx_t k = n - r * (r + (idx_t)1) / (idx_t)2;

    /* a(0) = 1 (row r = 0 has a single entry 1) */
    if (r == (idx_t)0)
        return (seq_t)1;

    seq_t res = (seq_t)0;

    /* sum c(r,l) for l = k, k+1, k+2  as long as l ≤ r */
    for (idx_t l = k; l <= k + (idx_t)2 && l <= r; ++l) {
        seq_t term = binom((idx_t)2 * r, r - l);
        term = term * (seq_t)l;
        term = term / (seq_t)r;
        res += term;
    }

    return res;
}

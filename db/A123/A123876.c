/*
 * A123876  Riordan array (1/(1+2*x), x*(1+x)/(1+2*x)^2).
 * https://oeis.org/A123876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) (row index) *
 *            k = n - r*(r+1)/2 (column index) * T(r,k) = (-1)^(r-k) *
 *            Σ_{j=k}^{min(r,2*k)} binom(k,j-k) * binom(r,j) * 2^(r-j) * The
 *            triangle T(n,k) is defined by the given Riordan array
 * Range:     i8 n <= 15, i16 n <= 56, i32 n <= 193, i64 n <= 675,
 *            i128 n <= 2574, big any n
 * Helpers:   binom_n_k, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"
#include "../utils/pow2.h"

/* Find the row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t triangular_row(idx_t n)
{
    idx_t r = 0;
    while (( (__int128)(r + 1) * (r + 2) ) / 2 <= n) {
        ++r;
    }
    return r;
}

/* Compute T(r,k) using the combinatorial sum. */
static seq_t triangle_entry(idx_t r, idx_t k)
{
    seq_t sum = 0;
    idx_t j_min = k;
    idx_t j_max = r;
    if (j_max > 2 * k) j_max = 2 * k;

    seq_t sign = (( (r - k) & 1) ? -1 : 1 );

    for (idx_t j = j_min; j <= j_max; ++j) {
        seq_t term = binom_n_k(k, j - k);          /* C(k, j-k) */
        term = term * binom_n_k(r, j);             /* * C(r, j) */
        term = term * pow2(r - j);                 /* * 2^(r-j) */
        if (sign < 0) sum -= term;
        else          sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    idx_t k = n - r * (r + 1) / 2;
    return triangle_entry(r, k);
}

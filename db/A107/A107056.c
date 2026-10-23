/*
 * A107056  Matrix inverse of A103247, so that T(n,k) = C(n,k)*A010842(n-k),
 *          read by rows.
 * https://oeis.org/A107056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where the triangle T is defined by T(n,k) =
 *            n!/k! * Σ_{j=0}^{n-k} 2^(n-k-j)/(n-k-j)! The linear index n is
 *            converted to (row,col) by n = row(row+1)/2 + col, 0≤col≤row
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"      /* seq_t binom(idx_t N, idx_t K); */

static seq_t *fac = NULL;           /* fac[i] = i! */
static size_t fac_len = 0;          /* number of entries stored */
static size_t fac_cap = 0;          /* capacity of fac array */

/* extend factorial table to include up to inclusive 'upto' */
static void ensure_factorial(idx_t upto)
{
    if (fac == NULL) {
        fac_cap = 8;
        fac = (seq_t*)malloc(fac_cap * sizeof(seq_t));
        fac[0] = (seq_t)1;
        fac_len = 1;
    }
    while (fac_len <= (size_t)upto) {
        if (fac_len >= fac_cap) {
            fac_cap *= 2;
            fac = (seq_t*)realloc(fac, fac_cap * sizeof(seq_t));
        }
        fac[fac_len] = fac[fac_len - 1] * (seq_t)fac_len;
        fac_len++;
    }
}

/* find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t val = mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* locate row and column from linear index */
    idx_t row = row_of_index(n);
    idx_t col = n - row * (row + 1) / 2;
    idx_t m   = row - col;          /* m = n - k */

    /* factorial of m needed for the sum */
    ensure_factorial(m);
    seq_t fac_m = fac[m];

    /* compute the sum Σ_{j=0..m} 2^(m-j)/(m-j)! in integer form
       by the recurrence: term_0 = fac_m, term_{j} = term_{j-1} * 2 / j */
    seq_t term = fac_m;
    seq_t sum  = term;
    for (idx_t j = 1; j <= m; ++j) {
        term = term * (seq_t)2;
        term = term / (seq_t)j;    /* exact division */
        sum += term;
    }

    /* T(row,col) = C(row,col) * sum   */
    seq_t bin = binom(row, col);
    return bin * sum;
}

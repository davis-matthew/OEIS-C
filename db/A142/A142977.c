/*
 * A142977  Table of coefficients in the expansion of the rational function
 *          1/{(1-x)^2 - y*(1+x)^2}.
 * https://oeis.org/A142977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row,col) where a(n) is the entry on the (row,col)
 *            position of the triangle defined by T(r,c) = sum_{j=0..c} C(2*r,
 *            c-j) * C(2*r+j+1, j) The index n is mapped to (row,col) by
 *            writing d = largest integer with d*(d+1)/2 <= n (diagonal
 *            number) pos = n - d*(d+1)/2 row = d - pos col = pos This...
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the diagonal number d for index n:
   return the largest d such that d*(d+1)/2 <= n.  */
static idx_t diag_from_index(idx_t n)
{
    idx_t low = 0, high = 1;
    /* increase high until the triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* binary search between low and high */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

/* Compute binomial coefficient C(n,k) for idx_t n,k using
   multiplicative formula to keep intermediate values small. */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i); /* multiply */
        res = res / (seq_t)i;           /* divide - exact */
    }
    return res;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* 1. Locate the diagonal (row+col = d). */
    idx_t d = diag_from_index(n);

    /* 2. Position inside the diagonal. */
    idx_t triangular = (idx_t)((unsigned __int128)d * (d + 1) / 2);
    idx_t pos = n - triangular;          /* 0 <= pos <= d */

    /* 3. Convert to (row,col). */
    idx_t row = d - pos;
    idx_t col = pos;

    /* 4. Compute T(row,col) via the defining sum. */
    seq_t sum = 0;
    for (idx_t j = 0; j <= col; ++j) {
        seq_t term1 = binomial_multiplicative((idx_t)(2 * row), (idx_t)(col - j));
        seq_t term2 = binomial_multiplicative((idx_t)(2 * row + j + 1), (idx_t)j);
        sum += term1 * term2;
    }
    return sum;
}

/*
 * A154867  A triangular sequence of polynomial coefficients: p(x,n) =
 *          Sum[m^n*x^m/m!, {m, 0, Infinity}]/(x*Exp[x]); q(x,n)= If[n == 0,
 *          1, p(x, n) + x^n*p(1/x, n)].
 * https://oeis.org/A154867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned, tabl
 * Formula:   a(n) = t(r,c) where r = floor((sqrt(8n+1)-1)/2), c = n -
 *            r(r+1)/2, and t(r,c) = 1 if c=0 or c=r, otherwise t(r,c) =
 *            S(r,c+1)+S(r,r-c+1) with S(r,k) the Stirling numbers of the
 *            second kind. The sequence enumerates the coefficients of the
 *            triangle defined by q(x,n)
 * Range:     u8 n <= 29, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef unsigned __int128 u128;          /* for 128‑bit integer arithmetic */

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Find an upper bound */
    while ((u128)high * (high + 1) / (u128)2 <= n) high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((u128)mid * (mid + 1) / (u128)2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Offset of row r in the flattened Stirling array: r*(r+1)/2. */
static idx_t row_offset(idx_t r)
{
    return (idx_t)((u128)r * (r + 1) / (u128)2);
}

/* Global storage for Stirling numbers of the second kind. */
static seq_t *S = NULL;
static idx_t max_precomp = 0;   /* largest n precomputed in S */

/* Compute sequence value a(n). */
seq_t a(idx_t n)
{
    /* Determine the row r and column c. */
    idx_t r = find_row(n);
    if (r == 0) return 1;     /* only one element in row 0 */

    idx_t c = n - row_offset(r);

    /* Expand Stirling table if needed. */
    if (r > max_precomp) {
        idx_t new_size = row_offset(r + 1);   /* (r+1)(r+2)/2 entries */
        S = (seq_t *)realloc(S, new_size * sizeof(seq_t));
        /* Initialize base case if first time. */
        if (max_precomp == 0 && S != NULL)
            S[0] = 1;   /* S(0,0) = 1 */

        /* Compute new rows. */
        for (idx_t row = max_precomp + 1; row <= r; ++row) {
            idx_t base = row_offset(row);
            idx_t prev_base = row_offset(row - 1);
            for (idx_t k = 0; k <= row; ++k) {
                idx_t pos = base + k;
                if (k == 0) {
                    S[pos] = 0;                /* S(n,0) = 0 for n>0 */
                } else if (k == row) {
                    S[pos] = 1;                /* S(n,n) = 1 */
                } else {
                    S[pos] = S[prev_base + k] * ((seq_t)k) + S[prev_base + k - 1];
                }
            }
        }
        max_precomp = r;
    }

    /* Compute the desired triangle entry. */
    if (c == 0 || c == r)
        return 1;

    idx_t base = row_offset(r);
    return S[base + c + 1] + S[base + (r - c) + 1];
}

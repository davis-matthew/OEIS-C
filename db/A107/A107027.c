/*
 * A107027  Number triangle associated to the Riordan arrays
 *          (1/(1+x),x/(1+x)^k),k>=0.
 * https://oeis.org/A107027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row,col) where (row,col) is obtained from the n-th
 *            element of the row‑wise flattening of the triangle T, and T(n,k)
 *            = (n-k-1)*C((n-k)*k,k) - (n-k-2)*sum_{j=0..k} C((n-k)*k,j)
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the binomial coefficient helper */
#include <sys/types.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Helper: determine the row and column in the triangle from the
   one‑dimensional index n. */
static void row_col(idx_t n, idx_t *row, idx_t *col) {
    idx_t r = 0;
    /* Find the largest r such that r*(r+1)/2 <= n */
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    *row = r;
    *col = n - (r * (r + 1)) / 2;
}
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t r, k;
    row_col(n, &r, &k);           /* r = row, k = col */
    idx_t diff = r - k;           /* (n-k) in the formula */
    idx_t M     = diff * k;       /* (n-k)*k */

    /* Binomial coefficient C(M, k) */
    seq_t C = binomial(M, k);

    /* Compute the partial sum Σ_{j=0..k} C(M, j) */
    seq_t sum = (seq_t)0;
    for (idx_t j = 0; j <= k; ++j) {
        sum += binomial(M, j);
    }

    /* Signed multipliers */
    int64_t a_mul = (int64_t)diff - 1; /* (n-k-1) */
    int64_t b_mul = (int64_t)diff - 2; /* (n-k-2) */

    /* Evaluate the formula taking care of possible negative
       multipliers.  The final result is always positive. */
    if (a_mul >= 0 && b_mul >= 0) {
        /* Both multipliers non‑negative: term1 - term2 */
        seq_t term1 = (seq_t)a_mul * C;
        seq_t term2 = (seq_t)b_mul * sum;
        return term1 - term2;
    }
    if (a_mul >= 0 && b_mul < 0) {
        /* term1 positive, term2 negative: term1 + |b_mul|*sum */
        seq_t term1 = (seq_t)a_mul * C;
        seq_t term2 = (seq_t)(-b_mul) * sum;
        return term1 + term2;
    }
    /* The only remaining case for this triangle is diff = 0,
       where both multipliers are negative. */
    /* result = |b_mul|*sum - |a_mul|*C */
    seq_t term1 = (seq_t)(-b_mul) * sum;   /* |b_mul|*sum */
    seq_t term2 = (seq_t)(-a_mul) * C;      /* |a_mul|*C */
    return term1 - term2;
}

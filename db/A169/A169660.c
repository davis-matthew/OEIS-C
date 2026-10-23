/*
 * A169660  Triangle, read by rows, T(n, k) = binomial(n-1,k-1)*n!/k! +
 *          binomial(n-1, n-k)* n!/(n-k+1)! - n!.
 * https://oeis.org/A169660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, k) = binomial(n-1,k-1)*n!/k! + binomial(n-1,n-k)*
 *            n!/(n-k+1)! - n! (triangle, read by rows)
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 67, i64 n <= 191,
 *            i128 n <= 530, big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the flattened triangle element a(n) with offset 1.  */
#include "../utils/factorial.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for 0, but safe */

    /* Find the row r such that r*(r-1)/2 < n <= r*(r+1)/2 */
    idx_t low = 1, high = n * 2; /* high is safe upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */
    idx_t prev_sum = (idx_t)(((__int128)(r - 1) * r) / 2); /* entries before this row */
    idx_t col = n - prev_sum;          /* column index within the row (1‑based) */

    /* Compute the terms using the given formula */
    seq_t fac_n = factorial((idx_t)r);
    seq_t fac_k = factorial((idx_t)col);
    seq_t fac_nk1 = factorial((idx_t)(r - col + 1));

    seq_t C = binom(r - 1, col - 1);

    seq_t term1 = C * (fac_n / fac_k);
    seq_t term2 = C * (fac_n / fac_nk1);

    return term1 + term2 - fac_n;
}

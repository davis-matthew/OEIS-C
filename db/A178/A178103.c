/*
 * A178103  Coefficient array of orthogonal polynomials
 *          P(n,x)=x*P(n-1,x)-(2n-3)*P(n-2,x), P(0,x)=1,P(1,x)=x.
 * https://oeis.org/A178103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 56, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178103: Coefficients of orthogonal polynomials
   P(0,x)=1, P(1,x)=x, P(n,x)=x*P(n‑1,x)-(2n‑3)*P(n‑2,x).
   The sequence a(n) is the coefficients of the row r that begins at the
   triangular index r(r+1)/2, i.e.
        a(n) = coeff of x^k in P(r,x), where r(r+1)/2 ≤ n < (r+1)(r+2)/2
        and k = n - r(r+1)/2. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 s = (__int128)mid * (mid + 1) / 2;
        if (s <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* The position of the required coefficient in row r */
    idx_t j = n - (idx_t)((__int128)r * (r + 1) / 2);

    /* Row 0 and row 1 */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (j == 0 ? (seq_t)0 : (seq_t)1);

    /* Allocate previous rows */
    seq_t *prev2 = (seq_t*)malloc(sizeof(seq_t));   /* row 0 */
    prev2[0] = (seq_t)1;

    seq_t *prev1 = (seq_t*)malloc(2 * sizeof(seq_t));   /* row 1 */
    prev1[0] = (seq_t)0;
    prev1[1] = (seq_t)1;

    seq_t *curr = NULL;

    /* Build successive rows up to r */
    for (idx_t cur = 2; cur <= r; ++cur) {
        idx_t len = cur + 1;
        curr = (seq_t*)malloc(len * sizeof(seq_t));

        seq_t mult = (seq_t)(2 * cur - 3);
        for (idx_t k = 0; k < len; ++k) {
            seq_t a_prev1 = 0;
            if (k > 0 && k - 1 < cur) a_prev1 = prev1[k - 1];
            seq_t a_prev2 = 0;
            if (k < cur - 1) a_prev2 = prev2[k];
            curr[k] = a_prev1 - mult * a_prev2;
        }

        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* The answer is in prev1 (row r) at position j */
    seq_t result = prev1[j];

    free(prev2);
    free(prev1);
    return result;
}

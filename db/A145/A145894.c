/*
 * A145894  Triangle read by rows: T(n,k) is the number of permutations p of
 *          {1,2,...,n} such that j and p(j) are of the same parity for k
 *          values of j (0<=k<=n).
 * https://oeis.org/A145894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col), where the triangle T is T(2m,2k) = [m! *
 *            C(m,k)]^2 T(2m+1,2k+1) = [(m+1)! * C(m,k)]^2 / (k+1) All other
 *            entries are 0. The sequence is read row‑by‑row: a(n) is the nth
 *            element of the triangle starting with T(0,0)
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 108, u64 n <= 258,
 *            u128 n <= 642, big any n
 * Validated: standard, pass, 82 terms through n = 9782 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* find the row r such that start(r) = r*(r+1)/2 <= n < start(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;           /* column index */

    if ((r & 1U) == 0) {           /* even row: n = 2m */
        if (c & 1U)                /* odd column => 0 */
            return 0;
        idx_t m = r >> 1;
        idx_t k = c >> 1;
        seq_t fact_m = factorial(m);
        seq_t bin    = binomial(m, k);
        seq_t term   = fact_m * bin;    /* m! * C(m,k) */
        return term * term;            /* square */
    } else {                       /* odd row: n = 2m+1 */
        if ((c & 1U) == 0)          /* even column => 0 */
            return 0;
        idx_t m = (r - 1) >> 1;
        idx_t k = (c - 1) >> 1;
        seq_t fact_m1 = factorial(m + 1);
        seq_t bin     = binomial(m, k);
        seq_t term    = fact_m1 * bin;   /* (m+1)! * C(m,k) */
        term = term * term;             /* square */
        seq_t denom = (seq_t)(k + 1);
        return term / denom;            /* exact division */
    }
}

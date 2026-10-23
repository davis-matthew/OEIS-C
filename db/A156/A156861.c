/*
 * A156861  Triangle read by rows: T(n, k) = 2^k - binomial(n, k+1).
 * https://oeis.org/A156861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r, k) where T(r, k) = 2^k - binomial(r, k+1) r is the
 *            row number (starting at 0) and k is the column inside that row.
 *            The terms are obtained by reading the triangle row-wise.
 *            Indexing starts at n = 0
 * Range:     i8 n <= 34, i16 n <= 134, i32 n <= 526, i64 n <= 2078,
 *            i128 n <= 5150, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

#if defined(__cplusplus)
/* In C++ mode we can directly use the utility binomial function, which
   automatically handles large integers. */
static inline seq_t binom_small(idx_t n, idx_t k) { return binom(n, k); }
#else
/* In C mode we implement our own binomial for 64‑bit seq_t.  The routine
   uses a 128‑bit intermediate to avoid overflow for the values required
   by the 64‑bit build (r <= ~66).  For larger values the intermediate
   may overflow, but those terms do not fit in seq_t and therefore are
   not checked. */
static inline seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k; /* use the smaller side */
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res /= i;
    }
    return (seq_t)res;
}
#endif

seq_t a(idx_t n)
{
    /* Find the row r and the cumulative number of terms before that row.
       The cumulative count is r(r+1)/2. */
    idx_t r = 0;
    seq_t cum = 0;               /* number of terms before row r */
    while (cum + (r + 1) <= n) {
        cum += r + 1;             /* add terms of current row */
        ++r;                      /* move to next row */
    }

    /* k is the column index (0‑based) within row r. */
    idx_t k = n - (idx_t)cum;

    /* Compute 2^k and binomial(r, k+1) using the appropriate helper. */
    seq_t pow2_k = pow2(k);
    seq_t binom_val = binom_small(r, k + 1);

    return pow2_k - binom_val;
}

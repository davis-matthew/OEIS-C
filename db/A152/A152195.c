/*
 * A152195  Triangle read by rows, A000012 * A152194.
 * https://oeis.org/A152195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{k=2c}^{r} C(r,k), where r is the row containing the
 *            n‑th term of the triangle (rows have lengths L(r) = ⌊r/2⌋+1) and
 *            c = n – prefixCount(r‑1). prefixCount(r) = Σ_{i=0}^{r} L(i) = if
 *            r even: (r/2+1)² if r odd : (r/2+1)(r/2+2) . The sum equals the
 *            tail of the binomial coefficients of orde...
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   prefixCount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* sqrt() for estimating search range */
#include "../utils/prefixCount.h"

/* Binomial coefficient C(n,k)  (0 ≤ k ≤ n)  using multiplicative formula.
   The computations are performed in the seq_t type to avoid overflow. */
static seq_t binom(idx_t n, idx_t k)
{
    seq_t c = 1;
    for (idx_t i = 1; i <= k; ++i) {
        c *= (seq_t)(n - i + 1);
        c /= (seq_t)i;              /* exact division */
    }
    return c;
}

seq_t a(idx_t n)
{
    /* --- locate the row r containing index n ---------------------- */
    /* approximate upper bound for r : r ≈ 2*sqrt(n) + 5 */
    double sq = sqrt((double)n);
    idx_t lo = 0;
    idx_t hi = (idx_t)(2.0 * sq + 5.0);
    if (hi < 1) hi = 1;

    /* ensure hi is large enough */
    while (prefixCount(hi) <= n) hi <<= 1;

    /* binary search for the smallest r with prefixCount(r) > n */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (prefixCount(mid) > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* number of terms before this row */
    idx_t prev = (r == 0) ? 0 : prefixCount(r - 1);
    /* column index within row */
    idx_t c = n - prev;                /* 0 ≤ c < L(r) */

    /* --- compute the required binomial tail sum -------------------- */
    idx_t a_start = 2 * c;             /* lower limit of the sum */
    seq_t sum = 0;

    if (a_start <= r) {
        seq_t comb = binom(r, a_start);   /* C(r, 2c) */

        for (idx_t k = a_start; k <= r; ++k) {
            sum += comb;
            if (k < r)
                comb = comb * (seq_t)(r - k) / (seq_t)(k + 1);  /* next binomial */
        }
    }

    return sum;
}

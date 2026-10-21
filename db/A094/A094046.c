/*
 * A094046  Triangle read by rows: T(n,k) is the number of noncrossing
 *          connected graphs on n nodes on a circle, having exactly k
 *          four-sided faces, n>=2, 0<=k<=floor(n/2)-1.
 * https://oeis.org/A094046
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 31, u64 n <= 133, u128 n <= 442,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A094046: flattened triangle of noncrossing connected graphs.
   For each queried index n >= 2 this function finds the row r and column k
   such that n-1 = Σ_{i=2}^{r-1} ⌊i/2⌋ + k + 1,
   then evaluates T(r,k) by the explicit formula
   T(r,k) = C(r+k-2,k) * ( Σ_{i=0..⌊(r-2k-2)/3⌋} C(r+k+i-2,i) * C(4r-4-k-i,
                 r-2k-2-3i) ) / (r-1). */
#include "../utils/binomial.h"

static seq_t compute_T(idx_t n, idx_t k) {
    /* T(n,k) = C(n+k-2,k) * ( Σ_{i=0..⌊(n-2k-2)/3⌋}
                            C(n+k+i-2,i) * C(4n-4-k-i, n-2k-2-3i) )
               -------------------------------------------------
                     n-1
          (integer division is exact) */
    seq_t term   = binomial(n + k - 2, k);
    seq_t sum    = 0;
    idx_t max_i  = (n - 2*k - 2) / 3;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t a = binomial(n + k + i - 2, i);
        seq_t b = binomial(4*n - 4 - k - i,
                           n - 2*k - 2 - 3*i);
        sum = sum + a * b;
    }
    seq_t prod  = term * sum;
    seq_t result = prod / (seq_t)(n - 1);
    return result;
}

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 2.  Map n to the flattened
       triangle index s = n-1 (1-based).  Find the smallest r
       such that floor(r^2/4) >= s, then k = s - floor((r-1)^2/4) - 1. */
    if (n < 2) return 0;       /* undefined for n<2; return 0 as placeholder */

    idx_t s = n - 1;            /* 1‑based position in the flattened array */

    /* binary search for r */
    idx_t lo = 2, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        idx_t cum = (mid * mid) / 4;     /* floor(mid^2/4) */
        if (cum >= s) hi = mid;
        else          lo = mid + 1;
    }
    idx_t r = lo;
    idx_t prev_cum = ((r - 1) * (r - 1)) / 4;
    idx_t k = s - prev_cum - 1;          /* 0-based column index */

    return compute_T(r, k);
}

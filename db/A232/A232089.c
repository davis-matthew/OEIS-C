/*
 * A232089  Table read by rows, which consist of 1 followed by 2^k, 0 <= k < n
 *          ; n = 0,1,2,3,...
 * https://oeis.org/A232089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if k <= 1 a(n) = 2^(k-1) otherwise where k = n –
 *            r(r+1)/2 and r = ⌊(√(1+8n) – 1)/2⌋
 * Range:     u8 n <= 53, u16 n <= 169, u32 n <= 593, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Helper to compute 2^e (large exponent) */
#include "../utils/pow2.h"
/*  Math functions for the square‑root */
#include <math.h>

seq_t a(idx_t n) {
    /* Find row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  */
    long double disc   = sqrtl((long double)1 + (long double)8 * (long double)n);
    idx_t     r        = (idx_t)((disc - 1) / 2);

    /* Correct possible rounding errors of the floating‑point estimate.  */
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    while (tri > n) {
        --r;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
        tri = (unsigned __int128)r * (r + 1) / 2;
    }

    /* Column index within the row */
    idx_t k = n - (idx_t)tri;

    /* Compute the term */
    if (k <= 1) {
        return (seq_t)1;
    } else {
        return pow2(k - 1);
    }
}

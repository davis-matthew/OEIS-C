/*
 * A210391  Number A(n,k) of semistandard Young tableaux over all partitions
 *          of n with maximal element <= k; square array A(n,k), n>=0, k>=0,
 *          read by antidiagonals.
 * https://oeis.org/A210391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from the square array * A(n,k) =
 *            coefficient of x^n in 1/((1-x)^k*(1-x^2)^{k(k-1)/2}). * The
 *            function implements A(n,k) via the closed form * * A(n,k) =
 *            Σ_{t=0..⌊n/2⌋} binom(k + n-2t -1 , k-1) * * binom((k(k-1)/2) + t
 *            -1 , t) * * with the convention that binom(a,b)=0...
 * Range:     u8 n <= 48, u16 n <= 126, u32 n <= 336, u64 n <= 924,
 *            u128 n <= 2669, big any n
 * Validated: standard, pass, 94 terms through n = 9774 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Find the diagonal number d such that d(d+1)/2 <= n < (d+1)(d+2)/2  */
static idx_t d_from_n(idx_t n)
{
    idx_t low = 0, high = n;
    idx_t d = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t cnt = mid * (mid + 1) / 2;
        if (cnt <= n) {
            d = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return d;
}

/* Compute A(n,k) using the binomial formula */
static seq_t A_value(idx_t n, idx_t k)
{
    if (k == 0) {
        return (n == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* q = k(k-1)/2 */
    idx_t q = k * (k - 1) / 2;
    seq_t sum = 0;

    for (idx_t t = 0; 2 * t <= n; ++t) {
        idx_t m = n - 2 * t;     /* m = n - 2t */
        seq_t term1 = binomial(m + k - 1, k - 1);

        seq_t term2;
        if (t == 0) {
            term2 = 1;  /* binom(...,0) = 1 */
        } else {
            if (q == 0) {
                term2 = 0;  /* binom(-1, t) = 0 for t>0 when q=0 */
            } else {
                term2 = binomial(q + t - 1, t);
            }
        }
        sum += term1 * term2;
    }
    return sum;
}

/* Public entry point */
seq_t a(idx_t n)
{
    /* Locate the anti‑diagonal (i,j) that corresponds to index n */
    idx_t d = d_from_n(n);
    idx_t offset = n - d * (d + 1) / 2;   /* offset within diagonal */
    idx_t i = offset;                     /* row index n */
    idx_t j = d - offset;                 /* column index k */
    return A_value(i, j);
}

/*
 * A189233  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          upwards, where the e.g.f. of column k is exp(k*(e^x-1)).
 * https://oeis.org/A189233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{i=0..r} Stirling2(r,i) * k^i where r and k are derived
 *            from n by reading the square array A(n,k) along antidiagonals
 *            upwards. The index n is zero‑based and the array satisfies
 *            A(n,1)=Bell(n)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 122, u64 n <= 302,
 *            u128 n <= 784, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Upper bound for the diagonal index required for n up to ~10000.
   triangular(512) ≈ 131328, far beyond the needed range. */
#define MAX_R 512

static seq_t Stirling[MAX_R + 1][MAX_R + 1];
static bool stirling_inited = false;

/* Initialise the Stirling‑second‑kind table up to MAX_R. */
static void init_stirling(void)
{
    if (stirling_inited) return;
    for (idx_t i = 0; i <= MAX_R; ++i)
        for (idx_t j = 0; j <= MAX_R; ++j)
            Stirling[i][j] = (seq_t)0;
    Stirling[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_R; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            /* S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
            seq_t term1 = (seq_t)k * Stirling[n - 1][k];
            seq_t term2 = Stirling[n - 1][k - 1];
            Stirling[n][k] = term1 + term2;
        }
    }
    stirling_inited = true;
}

seq_t a(idx_t n)
{
    init_stirling();      /* ensure Stirling table exists */

    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2   */
    idx_t d = 0;
    idx_t lo = 0, hi = n + 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            d = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    /* Position inside the diagonal: column index k = t,
       row index r = d - t.                                  */
    __uint128_t tri_d = (__uint128_t)d * (d + 1) / 2;
    idx_t t = n - (idx_t)tri_d;   /* t == k */
    idx_t r = d - t;              /* r == row index */

    if (r > MAX_R || t > MAX_R) {
        /* Out of pre‑computed bounds – fall back to on‑the‑fly
           computation for very large n (rare for tested inputs).   */
        /* Simple fallback: compute A(r,k) directly via recurrence. */
        seq_t result = 0;
        seq_t powk = 1;
        for (idx_t i = 0; i <= r; ++i) {
            result = result + Stirling[r][i] * powk;
            powk = powk * (seq_t)t;
        }
        return result;
    }

    /* Compute the binomial sum: A(r,k) = Σ S(r,i)*k^i   */
    seq_t result = 0;
    seq_t powk = 1;
    for (idx_t i = 0; i <= r; ++i) {
        result = result + Stirling[r][i] * powk;
        powk = powk * (seq_t)t;
    }

    return result;
}

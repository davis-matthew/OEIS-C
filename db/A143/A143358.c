/*
 * A143358  Triangle read by rows: T(n,k) = 2^k*binomial(n,k)*binomial(n-k,
 *          floor((n-k)/2)), 0 <= k <= n.
 * https://oeis.org/A143358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 196, u64 n <= 644,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the flattening of the triangle T(n,k) = 2^k 
 *          * binomial(n,k) * binomial(n-k, floor((n-k)/2)) read row‑wise.
 *          For a given linear index n, let 
 *          r = floor( (sqrt(8*n+1) - 1) / 2 ),
 *          k = n - r*(r+1)/2.
 *          Then a(n) = 2^k * binomial(r,k) * binomial(r-k, floor((r-k)/2)).
 *
 * The implementation uses only integer arithmetic and does not assume
 * calls come in any particular order.  It works for all the supported
 * seq_t / idx_t types, including arbitrary‑precision builds.
 */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Utility functions for the implementation: not external dependencies.      */
/* -------------------------------------------------------------------------- */

/* Compute floor(sqrt(x)) for an unsigned 64‑bit integer. */
static inline idx_t _floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * mid > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

/* Compute binomial(a,b) exactly using a multiplicative formula.   */
static inline seq_t _binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b == 0 || b == a) return 1;
    if (b > a - b) b = a - b;
    seq_t result = 1;
    for (idx_t i = 1; i <= b; ++i)
    {
        result = result * (a - b + i);
        result = result / i;
    }
    return result;
}

/* Compute 2^exp as seq_t.                                          */
static inline seq_t _pow2(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= 2;
    return result;
}

/* -------------------------------------------------------------------------- */
/* Main function required by the build.                                         */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t disc = 8 * n + 1;
    idx_t s = _floor_sqrt_u64(disc);
    idx_t r = (s - 1) / 2;

    /* Column k within the row. */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute the triangle entry T(r,k). */
    idx_t m = r - k;                 /* n-k in the formula */
    idx_t h = m / 2;                 /* floor((n-k)/2) */
    seq_t pow2k   = _pow2(k);
    seq_t bin_r_k = _binom(r, k);
    seq_t bin_m_h = _binom(m, h);

    return pow2k * bin_r_k * bin_m_h;
}

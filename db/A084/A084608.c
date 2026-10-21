/*
 * A084608  Triangle, read by rows, where the n-th row lists the (2n+1)
 *          coefficients of (1+2*x+3*x^2)^n.
 * https://oeis.org/A084608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^{n - r^2} in (1+2*x+3*x^2)^r, where r =
 *            floor(sqrt(n)). This is the triangle read by rows of
 *            (1+2*x+3*x^2)^n
 * Range:     u8 n <= 20, u16 n <= 70, u32 n <= 210, u64 n <= 757,
 *            u128 n <= 2667, big any n
 * Helpers:   binom, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Integer square‑root: floor(sqrt(n)) for 0 <= n <= 2^64-1 */
static idx_t isqrt(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (n >> 1) + 1;            /* high bound for the root */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                           /* lo is one above the root */
}

seq_t a(idx_t n)
{
    /* row index r and column index k within that row */
    idx_t r = isqrt(n);
    idx_t k = n - r * r;                 /* n - r^2 */
    seq_t sum = 0;

    /* compute T(r,k) = Σ_{j=0}^{⌊k/2⌋} binom(r, k-j) * binom(k-j, j)
       * 2^{k-2j} * 3^j */
    for (idx_t j = 0; j <= k >> 1; ++j) {
        idx_t kmj = k - j;                /* k - j */
        idx_t exp2 = k - (j << 1);       /* k - 2j */
        seq_t term = binom(r, kmj);
        term *= binom(kmj, j);
        term *= pow2(exp2);
        term *= pow3(j);
        sum += term;
    }
    return sum;
}

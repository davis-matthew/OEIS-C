/*
 * A143499  Triangle of unsigned 4-Lah numbers.
 * https://oeis.org/A143499
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth term of the concatenation of rows of the *
 *            unsigned 4‑Lah triangle T(n,k) (n,k≥4). Each row n lists * k =
 *            4,…,n in increasing order. The entry is * * T(n,k) =
 *            (n−4)!/(k−4)! · binomial(n+3,k+3) * * The function handles any n
 *            ≥ 4, returning 0 for n < 4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root, floor(sqrt(x)), 64‑bit inputs. */
static inline idx_t floor_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Compute binomial coefficient C(n,k) with n,k >= 0, k <= n.
 * The result is an integer; division is exact at every step. */
static inline seq_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* offset 4; first term is a(4) */

    /* Zero‑based index within the concatenated triangle. */
    idx_t i = n - 4;                /* i = 0 gives first element (T(4,4)) */

    /* Find the row number m.
       Let t = m−3.  We need the smallest t such that t(t+1)/2 > i. */
    uint64_t disc = 1 + 8ULL * (uint64_t)i;
    idx_t sqrtDisc = floor_sqrt_u64(disc);
    idx_t t = (sqrtDisc - 1) / 2;   /* floor of the expression */
    while (((unsigned __int128)t * (t + 1) / 2) <= i) ++t;
    idx_t m = t + 3;                /* row index in the triangle */

    /* Offset within the row (zero‑based). */
    unsigned __int128 prevFull = (unsigned __int128)(t - 1) * t / 2;  /* S(m−1) */
    idx_t prev = (idx_t)prevFull;
    idx_t offset = i - prev;        /* 0 ≤ offset < (m−3) */
    idx_t k = 4 + offset;           /* column index */

    /* Compute T(m,k) = (m−4)!/(k−4)! · binomial(m+3,k+3). */
    idx_t start = k - 3;               /* first factor of the ratio      */
    idx_t end   = m - 4;               /* last  factor of the ratio      */
    seq_t ratio = 1;
    for (idx_t x = start; x <= end; ++x)
        ratio *= (seq_t)x;

    seq_t bin = binom_nk(m + 3, k + 3);
    return bin * ratio;
}

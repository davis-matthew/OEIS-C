/*
 * A134060  Triangle T(n,k) = A124927(n,k) + A134058(n,k) - A007318(n,k), read
 *          by rows.
 * https://oeis.org/A134060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*binomial(r,k) - [r==0] - [k==0], where r is the row
 *            number such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2. The triangle entry is T(r,k) = 3*C(r,k)-[k=0]-[r=0]
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 608, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t used in idx_t, if needed */

static inline idx_t find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* grow hi until the triangular number exceeds n */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;
    /* binary search for the largest r with T(r) ≤ n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* triangular number r*(r+1)/2 computed safely in 128‑bit */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)((unsigned __int128)r * (r + 1) / 2);
}

/* multiplicative binomial coefficient: binom(r,k) */
static inline seq_t binom(idx_t r, idx_t k)
{
    idx_t kk = k;
    if (kk > r - kk)
        kk = r - kk;            /* use symmetry */
    seq_t res = 1;
    /* compute product_{i=1..kk} (r-kk+i) / i,
       each division is exact */
    for (idx_t i = 1; i <= kk; ++i) {
        res = res * (seq_t)(r - kk + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r   = find_row(n);               /* row number   */
    idx_t t   = triangular(r);             /* first index of this row */
    idx_t k   = n - t;                     /* column index */

    seq_t val = binom(r, k);               /* C(r,k)       */
    val = val * (seq_t)3;                  /* 3*C(r,k)     */

    if (r == 0)
        val -= (seq_t)1;                   /* subtract [r==0] */
    if (k == 0)
        val -= (seq_t)1;                   /* subtract [k==0] */

    return val;
}

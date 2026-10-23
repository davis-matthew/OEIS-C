/*
 * A131127  Table read by rows: 2*A007318(n,m) - A167374(n,m).
 * https://oeis.org/A131127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let r be the integer such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
 *            * Let m = n - r*(r+1)/2 (0 <= m <= r) * Then * if m == r a(n) =
 *            1 * else if m == r-1 a(n) = 2*r + 1 * else a(n) = 2 * C(r, m) *
 *            where C(r, m) is the binomial coefficient
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 8708, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial coefficient C(n, k) using gcd-based reduction
   (works for integer types and for arbitrary-precision seq_t). */
static seq_t binom_coef(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;                     /* den should now be 1 */

        res *= num;                    /* res becomes C(n, i) */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t tri_r = (r * (r + 1)) / 2;
    idx_t m   = n - tri_r;

    if (m == r) {            /* main diagonal */
        return (seq_t)1;
    }
    if (m == r - 1) {        /* sub‑diagonal */
        return (seq_t)(2 * r + 1);
    }

    /* general case */
    seq_t c = binom_coef(r, m);   /* C(r, m) */
    return c + c;                 /* 2*C(r, m) */
}

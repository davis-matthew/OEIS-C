/*
 * A146958  A functionally symmetric Polynomial as a triangle of coefficients:
 *          p(x,n)=If[n == 0, 1, (x + 1)^n + 2^(n - 3)*Sum[(3^(m-1) + 2*m+1
 *          )*x^m*(1 + x^(n - 2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^m in p(x,r) = (x + 1)^r + 2^(r-3) *
 *            Σ_{m=1}^{r-1} (3^(m-1) + 2*m + 1) * x^m * (1 + x^(r-2*m)), where
 *            r is the unique integer satisfying r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 *            and m = n – r(r+1)/2. For r = 0,1,2 the polynomial is defined by
 *            hand (rows {1}, {1,1} and {1,6,1} resp...
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t */

/* ---------- helper functions ---------- */

/* integer square‑root of a 64‑bit unsigned integer (floor) */
static idx_t floor_sqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, mid, ans = 0;
    while (lo <= hi)
    {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return ans;
}

/* binomial coefficient C(a,b) for unsigned integers (a,b ≤ 2^64-1)       */
static seq_t binom_uint(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b == 0 || b == a) return (seq_t)1;
    if (b > a - b) b = a - b;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i)
    {
        res *= (a - b + i);
        res /= i;
    }
    return res;
}

/* 2^exp as seq_t */
static seq_t pow2_uint(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)2;
    return res;
}

/* 3^exp as seq_t */
static seq_t pow3_uint(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)3;
    return res;
}

/* coefficient t(r,m) of x^m in p(x,r) (r ≥ 3) */
static seq_t coeff_tri(idx_t r, idx_t m)
{
    /* binomial part */
    seq_t res = binom_uint(r, m);

    /* contributions from the sum term */
    seq_t two = pow2_uint(r - 3);
    seq_t add = (seq_t)0;

    /* first part (m) */
    if (m >= 1 && m <= r - 1)
    {
        seq_t term = pow3_uint(m - 1) + (seq_t)((2 * m + 1));
        add += term;
    }

    /* second part (r - m) */
    idx_t t = r - m;
    if (t >= 1 && t <= r - 1)
    {
        seq_t term = pow3_uint(t - 1) + (seq_t)((2 * t + 1));
        add += term;
    }

    return res + two * add;
}

/* ---------- main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* row 0 */

    /* find row r: r(r+1)/2 ≤ n < (r+1)(r+2)/2    */
    idx_t s = floor_sqrt_u64((idx_t)(8 * n + 1));
    idx_t r = (s - 1) / 2;

    /* correct for off‑by‑one due to floor */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t m = n - r * (r + 1) / 2;   /* column offset in the row */

    /* special rows 0,1,2 */
    if (r == 0) return (seq_t)1;                /* row {1} */
    if (r == 1) return (seq_t)1;                /* row {1,1} : m is 0 or 1 */
    if (r == 2)                                  /* row {1,6,1} */
    {
        if (m == 1) return (seq_t)6;
        return (seq_t)1;
    }

    /* general case r ≥ 3 */
    return coeff_tri(r, m);
}

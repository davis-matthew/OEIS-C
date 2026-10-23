/*
 * A143256  Triangle read by rows, matrix multiplication A051731 * A128407 *
 *          A127648, 1<=k<=n.
 * https://oeis.org/A143256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = μ(k) * k if k divides r, else 0 where r = minimal integer
 *            with r(r+1)/2 ≥ n, k = n - (r-1)r/2 (column index of the n‑th
 *            element in the triangle)
 * Range:     i8 n <= 8384, i16 n <= 9870, i32 n <= 9870, i64 n <= 9870,
 *            i128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime‑free Mobius function (returns 1, -1, or 0) */
static int8_t mobius(uint64_t x)
{
    if (x == 1) return 1;
    int cnt = 0;
    uint64_t t = x;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int c = 0;
            while (t % p == 0)
            {
                t /= p;
                ++c;
                if (c > 1)          /* squared prime factor */
                    return 0;
            }
            ++cnt;                  /* distinct prime factor found */
        }
    }
    if (t > 1) ++cnt; /* remaining prime factor */

    return (cnt & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard anyway */

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t lo = 1, hi = 2;
    /* enlarge upper bound exponentially until it covers n */
    while (((__uint128_t)hi * (hi + 1)) / 2 < n)
        hi <<= 1;

    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index k in this row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                   /* 1 ≤ k ≤ r */

    /* If k does not divide r, the entry is 0 */
    if (r % k != 0) return 0;

    int8_t mu_k = mobius((uint64_t)k);
    seq_t val = (seq_t)k * (seq_t)mu_k;
    return val;
}

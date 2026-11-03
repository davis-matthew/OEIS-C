/*
 * A102892  Triangle read by rows: T(n,k) is the number of noncrossing trees
 *          with n edges in which the number of edges from the root to the
 *          first branch point is k.
 * https://oeis.org/A102892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k), where * r is the unique non‑negative integer
 *            such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, * k = n - r*(r+1)/2. *
 *            T(r, k) is defined by * T(r,0) = 5 * binomial(3r-1, r-2) /
 *            (3r-1) (r > 0) * T(r,r) = 2^(r-1) (r > 0) * T(r,k) =
 *            (2^(k-1)/(r-k+1)) * binomial(3r-3k+1, r-k) * – (2^k/(r-k)) *...
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 119, u64 n <= 377,
 *            u128 n <= 1274, big any n
 * Validated: standard, pass, 71 terms through n = 1261 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Binomial coefficient binomial(N,K) for non‑negative integers,
   computed multiplicatively.  It is exact for all values that
   fit in seq_t; intermediate values are kept within seq_t by
   dividing after each multiplication.  */
static seq_t
binomial(idx_t N, idx_t K)
{
    if (K > N)
        return (seq_t)0;
    if (K > N - K)
        K = N - K;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i) {
        res = (res * (N - K + i)) / (idx_t)i;
    }
    return res;
}

/* power of two: 2^exp */
static seq_t pow2(idx_t exp)
{
    seq_t r = (seq_t)1;
    while (exp != 0) {
        r *= (seq_t)2;
        exp--;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Determine the row r and position k within the row. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* k == 0 (for r > 0) */
    if (k == 0 && r > 0)
    {
        seq_t bin = binomial(3 * r - 1, r - 2);
        seq_t num = bin * 5;
        seq_t denom = (seq_t)(3 * r - 1);
        return num / denom;
    }

    /* k == r (for r > 0) */
    if (k == r)
    {
        return pow2(r - 1);
    }

    /* 0 < k < r */
    seq_t bin1 = binomial(3 * r - 3 * k + 1, r - k);
    seq_t part1 = (pow2(k - 1) * bin1) / (seq_t)(r - k + 1);

    seq_t bin2 = binomial(3 * r - 3 * k - 2, r - k - 1);
    seq_t part2 = (pow2(k) * bin2) / (seq_t)(r - k);

    return part1 - part2;
}

/*
 * A051678  Square-pyramid-tree numbers: a(n) = sum(b(m),m=1..n), b(m) = 1^2,
 *          1^2,2^2, 1^2,2^2,3^2,.. = (A002260)^2.
 * https://oeis.org/A051678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = ∑_{k=1..n} b(k) where b(k) = 1², 1², 2², 1², 2², 3², … .
 *            Using the triangular representation from OEIS: a(n) = 1/12·t·(t
 *            + 2)·(t + 1)² + 1/6·m·(m + 1)·(2m + 1) where t = floor((√(8n +
 *            1) – 1)/2) and m = n – t(t + 1)/2. This formula is computed
 *            directly in integer arithmetic
 * Range:     u8 n <= 26, u16 n <= 432, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 64‑bit unsigned integer (floor). */
static inline unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long lo = 0, hi = x, mid, res = 0;
    while (lo <= hi)
    {
        mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > x / mid)
        {
            hi = mid - 1;
        }
        else
        {
            res = mid;
            lo = mid + 1;
        }
    }
    return res;
}

/* Sequence a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset 1: behaviour for n=0 irrelevant */

    /* Convert to 64‑bit for intermediate arithmetic (safe for n up to ~10k). */
    uint64_t nu = (uint64_t)n;
    uint64_t v = 8ULL * nu + 1ULL;
    uint64_t sqrtv = isqrt64(v);
    uint64_t t = (sqrtv - 1ULL) / 2ULL;                /* triangular block index */
    uint64_t Tt = t * (t + 1ULL) / 2ULL;              /* T_t = t*(t+1)/2 */
    uint64_t m = nu - Tt; /* offset within the block */

    /* First term: T_(t+1) = t*(t+2)*(t+1)²/12  (value of a at index T_t). */
    seq_t first = (seq_t)t * (t + 2ULL) * (t + 1ULL) * (t + 1ULL) / 12ULL;

    /* Sum of squares up to m: m*(m+1)*(2m+1)/6. */
    seq_t sumsq = (seq_t)m * (m + 1ULL) * (2ULL * m + 1ULL) / 6ULL;

    return first + sumsq;
}

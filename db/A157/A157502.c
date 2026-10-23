/*
 * A157502  Even numbers without the squares.
 * https://oeis.org/A157502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + round(sqrt(2*n)) - (round(sqrt(2*n)) mod 2)
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(sqrt(x)) for a 64‑bit integer using binary search. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32, mid;
    while (lo < hi) {
        mid = (lo + hi + 1) >> 1;           /* upper mid */
        if (mid <= x / mid)                  /* mid*mid <= x without overflow */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* t = 2*n, s = floor_sqrt(t) */
    idx_t t = n + n;
    idx_t s = isqrt_u64(t);

    /* d = t - s^2 */
    idx_t d = t - s * s;

    /* Round sqrt: R = s if d <= s else s+1 */
    idx_t R = (d <= s) ? s : s + 1;

    /* Compute a(n) = 2*n + R - (R mod 2) */
    seq_t res = (seq_t)2 * (seq_t)n + (seq_t)R;
    if (R & 1)
        res -= (seq_t)1;
    return res;
}

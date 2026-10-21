/*
 * A081757  Number of ways to write n as i*j+i-j, 0<i<j<=n.
 * https://oeis.org/A081757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A081757:
 * a(n) = number of divisors d of n-1 that are strictly smaller than ⌊√(n-1)⌋
 * (for n > 1, a(1)=0). */

/* Integer floor square root for type idx_t (at most 64‑bit unsigned). */
static inline idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);     /* hi > sqrt(2^64-1)   */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n may be as large as 2^64‑1; handle small cases quickly. */
    if (n <= 1) return (seq_t)0;
    idx_t m = n - 1;                  /* m = n - 1 */
    if (m <= 1) return (seq_t)0;      /* m == 1 -> no divisor < 1 */

    idx_t r = floor_sqrt(m);          /* r = ⌊√m⌋ */
    seq_t count = 0;                  /* number of divisors < r */

    for (idx_t i = 1; i < r; ++i) {
        if (m % i == 0)
            ++count;
    }
    return count;
}

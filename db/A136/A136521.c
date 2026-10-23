/*
 * A136521  Triangle read by rows: (1, 2, 2, 2, ...) on the main diagonal and
 *          the rest zeros.
 * https://oeis.org/A136521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = 1 if n==0; a(n) = 2 if n = r*(r+3)/2 for some integer
 *            r≥1; otherwise 0
 * Range:     u8 n <= 350, u16 n <= 350, u32 n <= 350, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;           /* fits in idx_t for all x ≤ 2^64-1 */
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    idx_t s = 9 + 8 * n;                 /* 9 + 8*n is always ≥ 17 for n>0 */
    idx_t t = isqrt_u64(s);              /* floor(sqrt(s)) */
    if (t * t != s)                      /* not a perfect square */
        return (seq_t)0;

    /* t = sqrt(9+8*n) is odd; check (t-3)/2 is integer ≥1 */
    if (((t - 3) & 1u) == 0) {
        idx_t r = (t - 3) / 2;
        if (r >= 1u)
            return (seq_t)2;
    }
    return (seq_t)0;
}

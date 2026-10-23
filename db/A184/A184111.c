/*
 * A184111  a(n) = n + floor(4*sqrt(n-1)); complement of A184112.
 * https://oeis.org/A184111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(4*sqrt(n-1))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n) {
    /* integer square root by binary search, 64‑bit */
    if (n < 2) return n;
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            res = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition */
    idx_t x = n - 1;                 /* n-1 */
    idx_t s = isqrt_u64(x);          /* floor(sqrt(n-1)) */

    unsigned __int128 L = (unsigned __int128)16 * (unsigned __int128)x; /* 16*(n-1) */

    idx_t base = s * 4;              /* 4*floor(sqrt(n-1)) */
    idx_t best = base;               /* current best integer part */

    for (idx_t k = 0; k <= 3; ++k) { /* possible fractional increments */
        unsigned __int128 t = (unsigned __int128)(base + k);
        if (L >= t * t)
            best = base + k;
    }

    return (seq_t)n + (seq_t)best;   /* final result */
}

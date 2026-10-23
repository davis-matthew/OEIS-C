/*
 * A115065  Number of points with integer coordinates inside the equilateral
 *          triangle with base [0,n].
 * https://oeis.org/A115065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{x=0}^{⌊√(3n²)/2⌋} (n + 1 - 2·⌈x/√3⌉)
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root for a 128‑bit unsigned integer: 0 ≤ n ≤ 2^127-1 */
static inline idx_t isqrt128(unsigned __int128 n)
{
    unsigned __int128 lo = 0, hi = 1;
    /* find an upper bound for the square root */
    while (hi * hi <= n) {
        hi <<= 1;
    }
    lo = hi >> 1;
    /* binary search between lo and hi */
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

/* Count lattice points in the equilateral triangle of side n
 * with base along the y‑axis from (0,0) to (0,n).               */
seq_t a(idx_t n)
{
    /* compute M = floor( sqrt(3) * n / 2 ) = floor( sqrt(3*n²) / 2 ) */
    unsigned __int128 val = (unsigned __int128)3 * n * n;
    idx_t M = isqrt128(val) / 2;

    seq_t total = 0;
    idx_t t = 0;             /* t = ceil(x/√3) */
    for (idx_t x = 0; x <= M; ++x) {
        if (x > 0) {
            /* increase t until t²*3 ≥ x², i.e. t = ceil(x/√3) */
            while ((unsigned __int128)t * t * 3 < (unsigned __int128)x * x) {
                ++t;
            }
        }
        /* term = n + 1 - 2 * t */
        seq_t term = (seq_t)(n + 1) - (seq_t)2 * (seq_t)t;
        total += term;
    }
    return total;
}

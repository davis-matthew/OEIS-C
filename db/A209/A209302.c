/*
 * A209302  Table T(n,k) = max{n+k-1, n+k-1} n, k > 0, read by sides of
 *          squares from T(1,n) to T(n,n), then from T(n,n) to T(n,1).
 * https://oeis.org/A209302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = sequence obtained by reading T(i,j)=i+j-1 by the sides of
 *            squares: for each side length s=1,2,3,... add
 *            T(1,s),T(2,s),...,T(s,s) then T(s,s-1),...,T(s,1)
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast integer square root (floor) for 64-bit unsigned integers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* sqrt(2^64-1) < 2^32          */
    /* binary search for highest m with m*m <= n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* offset 0 is not used, but guard anyway */
        return (seq_t)0;

    /* side length s = ceil(sqrt(n)) */
    idx_t sq = isqrt_u64(n);
    idx_t s = sq;
    if (sq * sq < n)   /* adjust for ceil */
        ++s;

    /* offset within this side */
    idx_t base = (s - 1) * (s - 1);       /* (s-1)^2 */
    idx_t offset = n - base;              /* 1 <= offset <= 2*s-1 */

    if (offset <= s) {                    /* top side */
        idx_t k = offset;                 /* 1 <= k <= s */
        return (seq_t)k + (seq_t)s - 1;
    } else {                              /* bottom side */
        idx_t t = offset - s;             /* 1 <= t <= s-1 */
        return (seq_t)2 * (seq_t)s - (seq_t)t - 1;
    }
}

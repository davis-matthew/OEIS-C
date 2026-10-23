/*
 * A189151  Numbers k such that k < floor(sqrt(k)) * ceiling(sqrt(k)).
 * https://oeis.org/A189151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((t*t + 5*t)/2) + n + 1, where t = floor(sqrt(2*n)+1/2)
 * Range:     u8 n <= 105, u16 n <= 9825, u32 n <= 9825, u64 n <= 9825,
 *            u128 n <= 9825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root for a 128‑bit value, returning the floor sqrt in uint64_t */
static uint64_t isqrt128(__int128 v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 33);   /* (1<<33)^2 > 2^65 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;   /* floor sqrt */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, but harmless */

    /* compute t = round(sqrt(2*n)) */
    __int128 two_n = (__int128)n * 2;
    uint64_t s = isqrt128(two_n);          /* floor sqrt(2*n)     */
    __int128 lhs = (__int128)4 * n;        /* 4*n                  */
    __int128 rhs = 2 * ((__int128)s * s + s) + 1;
    uint64_t t = (lhs > rhs) ? (s + 1) : s; /* round to nearest integer */

    /* a(n) = ((t*t + 5*t)/2) + n + 1 */
    __int128 tt = (__int128)t;
    __int128 part = (tt * tt + (__int128)5 * tt) / 2;
    seq_t result = (seq_t)(part + (__int128)n + 1);
    return result;
}

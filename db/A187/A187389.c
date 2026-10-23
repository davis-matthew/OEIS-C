/*
 * A187389  a(n) = floor(r*n), where r = 1 + sqrt(7) + sqrt(6); complement of
 *          A187390.
 * https://oeis.org/A187389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + sqrt(7) + sqrt(6)) * n)
 * Range:     u8 n <= 41, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for 128‑bit values: returns ⌊sqrt(x)⌋. */
static inline unsigned __int128
isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    /* 2^64 is an upper bound for the square root of any 128‑bit value. */
    unsigned __int128 hi = ((unsigned __int128)1) << 64;
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Beatty sequence for r = 1 + sqrt(7) + sqrt(6). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Off‑by‑offset safety, never used. */

    /* Work in 128‑bit to avoid overflow for n up to ~10^4. */
    unsigned __int128 N      = (unsigned __int128)n;
    unsigned __int128 N2     = N * N;                      /* n^2       */
    unsigned __int128 s6     = isqrt_uint128(6 * N2);      /* ⌊n√6⌋     */
    unsigned __int128 s7     = isqrt_uint128(7 * N2);      /* ⌊n√7⌋     */
    unsigned __int128 sum    = s6 + s7;                    /* floor part of n(√6+√7) */
    unsigned __int128 splus1 = sum + 1;                    /* sum+1     */

    /* Determine whether an extra carry is needed: floor(n(√6+√7)) = sum or sum+1. */
    unsigned __int128 d = splus1 * splus1 - 13 * N2;        /* (sum+1)² - 13n²  */

    unsigned carry = 0;
    if (d > 0) {
        /* Test 168 n⁴ ≥ d²  (derived from 42 ≥ (d/(2n²))²). */
        unsigned __int128 n4 = N2 * N2;                     /* n⁴        */
        unsigned __int128 lhs = (unsigned __int128)168 * n4;
        unsigned __int128 rhs = d * d;
        if (lhs >= rhs)
            carry = 1;
    } else {
        carry = 1;    /* d ≤ 0 ⇒ always carry */
    }

    /* Final result: n + sum + carry. */
    unsigned __int128 result128 = N + sum + carry;

    return (seq_t)result128;
}

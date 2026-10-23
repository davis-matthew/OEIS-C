/*
 * A024538  a(n) = [ n/{n*sqrt(2)} ], where {x} := x - [ x ].
 * https://oeis.org/A024538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n / { n*sqrt(2) } ) where {x} = x - [x]
 * Range:     u8 n <= 16, u16 n <= 168, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root for 128‑bit unsigned values.
 * Returns floor(sqrt(x)). */
static inline uint64_t floor_sqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    /* Hi is 2^64, its square is > 2^128, so we use division to avoid overflow. */
    unsigned __int128 hi = ((unsigned __int128)1) << 64;

    while (lo + 1 < hi)
    {
        unsigned __int128 mid = (lo + hi) >> 1;
        /* mid*mid <= x  ⇔  mid <= x / mid */
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid;
    }
    return (uint64_t)lo;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for offset = 1, safety */

    /* Work in unsigned __int128 for intermediate arithmetic. */
    unsigned __int128 n128 = (unsigned __int128)n;

    /* k = floor(n*sqrt(2)) = floor(sqrt(2*n^2)) */
    unsigned __int128 n2 = n128 * n128;
    unsigned __int128 t2 = (unsigned __int128)2 * n2;
    uint64_t k = floor_sqrt_uint128(t2);

    /* e = 2*n^2 - k^2  (always positive for n>=1) */
    unsigned __int128 k2 = (unsigned __int128)k * k;
    unsigned __int128 e128 = t2 - k2;          /* e fits in 64 bits */

    /* p = floor(n^2*sqrt(2)) = floor(sqrt(2*n^4)) */
    unsigned __int128 n4 = n2 * n2;            /* n^4 */
    unsigned __int128 p128 = (unsigned __int128)2 * n4;
    uint64_t p = floor_sqrt_uint128(p128);

    /* L = p + n*k  */
    unsigned __int128 L = (unsigned __int128)p + n128 * k;

    /* a(n) = floor( L / e )  */
    unsigned __int128 a128 = L / e128;

    return (seq_t)a128;
}

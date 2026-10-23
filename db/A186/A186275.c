/*
 * A186275  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          octagonal numbers. Complement of A186276.
 * https://oeis.org/A186275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( (2 + sqrt(4 + 12*T(n)) ) / 6 ) where T(n) =
 *            n(n+1)/2 is the nth triangular number. This gives the rank of
 *            the nth triangular number in the joint ranking of triangular and
 *            octagonal numbers (octagonal numbers are counted first when
 *            equal)
 * Range:     u8 n <= 22, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned __int128.
   Returns floor( sqrt(x) ).  Works for any x < 2^128.
   Since sqrt(2^128) < 2^64, the result fits in uint64_t. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64; /* hi = 2^64 */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (uint64_t)(lo - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1, but guard against 0 */

    /* Compute triangular number T(n) = n(n+1)/2 in seq_t arithmetic */
    seq_t tri = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* y = 4 + 12 * tri  (need 128‑bit to avoid overflow) */
    unsigned __int128 y = (unsigned __int128)4 + (unsigned __int128)12 * (unsigned __int128)tri;

    /* sqrt_y = floor( sqrt(y) ) */
    uint64_t sqrt_y = isqrt_uint128(y);

    /* Number of octagonal numbers <= tri */
    uint64_t oct_cnt = (sqrt_y + 2) / 6;

    /* a(n) = n + oct_cnt  (cast to seq_t) */
    return (seq_t)n + (seq_t)oct_cnt;
}

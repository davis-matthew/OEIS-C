/*
 * A106459  Expansion of f(-x, -x^3) in powers of x where f(,) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A106459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A106459
 *  a(n) = 0 unless n is a triangular number n = k(k+1)/2.
 *  If n is triangular, a(n) = (-1)^{⌊(k+1)/2⌋},
 *  i.e. a(n) = +1 for k ≡ 0 or 3 (mod 4), a(n) = –1 for k ≡ 1 or 2 (mod 4). */

#include <stdint.h>

/* integer square root for 128‑bit unsigned values */
static inline uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 64; /* sqrt(max_uint128) = 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return (uint64_t)low;
}

/* sequence generator */
seq_t a(idx_t n)
{
    /* compute t = 8*n + 1 as 128‑bit to avoid overflow for large n */
    unsigned __int128 t128 = (unsigned __int128)n * 8 + 1;

    /* integer sqrt of t128 */
    uint64_t sqrt_t = isqrt128(t128);

    /* candidate k such that k(k+1)/2 = n */
    uint64_t k = (sqrt_t - 1) / 2;

    /* verify triangularity: k(k+1)/2 == n */
    unsigned __int128 tri = (unsigned __int128)k * (k + 1) / 2;
    if (tri != n)
        return 0;

    /* determine sign: +1 if k%4 == 0 or 3, else -1 */
    uint64_t rem = k & 3ULL;          /* k % 4 */
    seq_t sign = (rem == 1 || rem == 2) ? -1 : 1;
    return sign;
}

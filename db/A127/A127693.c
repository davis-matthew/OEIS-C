/*
 * A127693  Expansion of psi(x^2) + x * psi(x^10) in powers of x where psi()
 *          is a Ramanujan theta function.
 * https://oeis.org/A127693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 exactly when n is of the form k(k+1) or 1 + 5*k(k+1)
 *            for some integer k ≥ 0; otherwise a(n) = 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root for a 64‑bit unsigned integer.
   Binary search on [0, 2^32‑1] using 128‑bit arithmetic to avoid overflow. */
#include <stdbool.h>

static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0;
    idx_t high = 0xFFFFFFFFULL;            /* sqrt(UINT64_MAX) < 2^32 */

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;

        if (sq == (unsigned __int128)v)
            return mid;
        if (sq < (unsigned __int128)v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Check for k(k+1) pattern.  Solve k^2 + k - n = 0. */
    idx_t d = 1 + 4 * n;
    idx_t s = isqrt_u64(d);
    bool tri = (s * s == d) && (((s - 1) & 1) == 0);

    /* Check for 1 + 5*k(k+1) pattern. */
    bool fifth = false;
    if (n >= 1 && ((n - 1) % 5) == 0) {
        idx_t m = (n - 1) / 5;          /* m = k(k+1) */
        idx_t d2 = 1 + 4 * m;
        idx_t s2 = isqrt_u64(d2);
        fifth = (s2 * s2 == d2) && (((s2 - 1) & 1) == 0);
    }

    return (seq_t)(tri || fifth);
}

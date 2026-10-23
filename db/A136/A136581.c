/*
 * A136581  Triangle read by rows: A136572 * A128174.
 * https://oeis.org/A136581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r! if (n - r(r+1)/2) has the same parity as r; otherwise
 *            a(n) = 0. Here r = ⌊(√(8n+1)-1)/2⌋
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: integer square root of uint64_t (floor). */
static inline idx_t int_isqrt_u64(idx_t x)
{
    /* Assume the build provides _floor_isqrt_u64 but fall back to
       a simple binary search if not available. */
#ifdef _floor_isqrt_u64
    return _floor_isqrt_u64(x);
#else
    idx_t lo = 0, hi = (idx_t)1 << 32;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
#endif
}

/* Compute factorial of m in seq_t.  For arbitrary‑precision seq_t
   this will grow as needed. */
static seq_t factorial_seq(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i) {
        res *= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n + 1) - 1) / 2) */
    idx_t t = (idx_t)((__int128)8 * n + 1);
    idx_t r = (int_isqrt_u64(t) - 1) / 2;

    /* offset = r*(r+1)/2 ; use 128‑bit to avoid overflow */
    idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t j = n - offset;

    /* Return r! if j and r have the same parity, otherwise 0. */
    if (((r ^ j) & 1) == 0) {
        return factorial_seq(r);
    } else {
        return 0;
    }
}

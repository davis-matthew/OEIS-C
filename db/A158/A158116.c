/*
 * A158116  Triangle T(n,k) = 6^(k*(n-k)), read by rows.
 * https://oeis.org/A158116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   compute a(n) from OEIS A158116 a(n) is the flattened triangle
 *            T(r,k) with T(r,k) = 6^(k*(r-k))
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 38, u64 n <= 59, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root: floor(sqrt(x)) for 64‑bit values */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32); /* > sqrt(UINT64_MAX) */

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * mid <= (unsigned __int128)x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* compute 6^exp where exp fits in idx_t */
static inline seq_t pow6(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= 6;
    return result;
}

seq_t a(idx_t n)
{
    /* special case n==0 to avoid the sqrt step (though it would work) */
    if (n == 0)
        return 1;

    /* row index r: largest r with (r+1)(r+2)/2 > n  →  r = floor((sqrt(8n+1)-1)/2) */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;

    /* position k inside that row */
    idx_t row_start = r * (r + 1) / 2; /* first index of row r */
    idx_t k = n - row_start;

    /* exponent for power of six: k * (r - k) */
    idx_t exp = k * (r - k);

    return pow6(exp);
}

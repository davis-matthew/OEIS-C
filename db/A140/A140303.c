/*
 * A140303  Triangle T(n,k) = 3^(n-k) read by rows.
 * https://oeis.org/A140303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^(r - (n - r*(r+1)/2)) where r =
 *            floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle is T(n,k) = 3^(n-k).  t(n,k) is read row‑wise. */
#include <stdint.h>

/* Determine the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
   Uses a binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t find_row(idx_t n)
{
    /* Find an upper bound for r by exponential search. */
    idx_t low = 0;
    idx_t high = 1;
    while (((__uint128_t)(high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;

    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

/* Compute a(n) for OEIS A140303.  No dynamic allocation is used. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);                      /* row containing n */
    __uint128_t tri = (__uint128_t)r * (r + 1) / 2;
    idx_t c     = n - (idx_t)tri;               /* offset within the row */
    idx_t exp   = r - c;                        /* exponent of 3 */

    /* Compute 3^exp using repeated multiplication.  Executed in O(exp). */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)3;
    }
    return result;
}

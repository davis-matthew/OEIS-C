/*
 * A051777  Triangle read by rows, where row (n) = n mod n, n mod (n-1), n mod
 *          (n-2), ...n mod 1.
 * https://oeis.org/A051777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = m mod (m - k) where m = ceil((sqrt(8n+1)-1)/2), k = n -
 *            m(m-1)/2 - 1 (0‑based offset in row)
 * Range:     u8 n <= 9727, u16 n <= 9727, u32 n <= 9727, u64 n <= 9727,
 *            u128 n <= 9727, big any n
 * Helpers:   sqrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_uint128.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the row number m such that m*(m+1)/2 >= n. */
    __uint128_t temp   = (__uint128_t)n * 8 + 1;          /* 8n + 1 */
    idx_t      sqrt_val = sqrt_uint128(temp);             /* floor(sqrt(8n+1)) */
    idx_t      m        = (sqrt_val - 1) / 2;              /* floor((sqrt-1)/2) */

    /* Adjust upward if necessary */
    while ((__uint128_t)m * (m + 1) / 2 < n)
        ++m;

    /* Number of elements before this row */
    __uint128_t sum_before = (__uint128_t)(m - 1) * m / 2;
    idx_t pos = n - (idx_t)sum_before - 1; /* 0‑based position in row */
    idx_t j   = m - pos;                    /* j = n - pos in row formula */

    /* The value is m mod j */
    return (seq_t)(m % j);
}

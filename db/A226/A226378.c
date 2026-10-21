/*
 * A226378  Number of distinct sums i+j+k with i,j,k >= 0, i*j*k = n.
 * https://oeis.org/A226378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums i+j+k with i,j,k >= 1 * such that
 *            i*j*k = n, and a(0) is defined as 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cbrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_cbrt_u64.h"

/* ---------- Integer square‑root for 64‑bit numbers ---------- */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    /* 2^32 is > sqrt(2^64-1), so it is an upper bound */
    idx_t high = (idx_t)1u << 32;
    if (high > n) high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 square = (__int128)mid * mid;
        if (square <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* OEIS defines a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* For n > 0 all factors are at least 1, so we never need i=0 */
    /* Maximum possible sum is 3*n (when i=j=k=n). */
    idx_t max_sum = 3 * n;
    size_t arr_size = (size_t)max_sum + 1;

    /* Boolean array marking which sums have been seen */
    bool *seen = (bool *)calloc(arr_size, sizeof(bool));
    if (seen == NULL)  /* memory failure – unlikely in this context */
        return (seq_t)0;

    seq_t count = 0;
    idx_t max_i = floor_cbrt_u64(n);

    for (idx_t i = 1; i <= max_i; ++i) {
        if (n % i != 0) continue;
        idx_t m = n / i;                     /* remaining product j*k */
        idx_t max_j = floor_sqrt_u64(m);

        for (idx_t j = i; j <= max_j; ++j) {
            if (m % j != 0) continue;
            idx_t k = m / j;                 /* ensures i <= j <= k */

            idx_t sum = i + j + k;
            /* sum fits in size_t because sum <= 3*n and n ≤ 2^64-1 */
            if (!seen[(size_t)sum]) {
                seen[(size_t)sum] = true;
                ++count;
            }
        }
    }

    free(seen);
    return count;
}

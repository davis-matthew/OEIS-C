/*
 * A126457  Triangle, read by rows, where T(n,k) = C( C(n+2,3) - C(k+2,3) + 3,
 *          n-k) for n>=k>=0.
 * https://oeis.org/A126457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C( C(r+2,3) - C(k+2,3) + 3, r-k ) where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2. This gives the
 *            triangle entries T(n,k) read by rows
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute n choose 3, safely for n up to 2^64-1. */
static inline idx_t comb3(idx_t n)
{
    if (n < 3) return 0;
    __uint128_t prod = (__uint128_t)n * (n - 1) * (n - 2) / 6;
    return (idx_t)prod;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = 1;
    while ((__uint128_t)high * (high + 1) / 2 <= n) high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;

    __uint128_t offset128 = (__uint128_t)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;
    idx_t k = n - offset;            /* 0 <= k <= r */

    idx_t C_r = comb3(r + 2);        /* C(r+2,3) */
    idx_t C_k = comb3(k + 2);        /* C(k+2,3) */

    idx_t A = C_r - C_k + 3;         /* binomial upper index */
    idx_t B = r - k;                 /* binomial lower index */

    return binom(A, B);
}

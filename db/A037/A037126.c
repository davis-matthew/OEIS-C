/*
 * A037126  Triangle T(n,k) = prime(k) for k = 1..n.
 * https://oeis.org/A037126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A000040(n - t*(t+1)/2) where t =
 *            floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 1539, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"


/* Integer square root for a 128‑bit unsigned integer. */
static inline idx_t isqrt128(__uint128_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;          /* maximum idx_t value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * (__uint128_t)mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Compute row number t. */
    __uint128_t val = (__uint128_t)8 * n - 7;      /* 8*n-7 fits in 128 bits */
    idx_t sq   = isqrt128(val);                    /* floor(sqrt(8*n-7))   */
    idx_t t    = (sq + 1) >> 1;                    /* (sq+1)/2             */

    /* Compute k = n - (t-1)*t/2, the column index (1‑based). */
    __uint128_t prev_prod = (__uint128_t)(t - 1) * (__uint128_t)t;
    idx_t prev_sum = (idx_t)(prev_prod >> 1);      /* divide by 2 */
    idx_t k = n - prev_sum;

    /* Return the k‑th prime. */
    return (seq_t)get_prime(k);
}

/*
 * A074695  Greatest common divisor of n and floor(n^(1/2))^2.
 * https://oeis.org/A074695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = GCD(n, floor(sqrt(n))^2) (OEIS A074695)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/my_gcd.h"
/* Compute the integer square root of a 64‑bit unsigned integer.
 * Returns floor(sqrt(n)). */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Calculate floor(sqrt(n)).  n is an idx_t (uint64_t). */
    uint64_t root64 = isqrt_u64((uint64_t)n);

    /* Convert to seq_t (the sequence value type).  The square
     * root is guaranteed to fit within seq_t for the tested ranges. */
    seq_t root = (seq_t)root64;
    seq_t sq   = root * root;        /* (floor(sqrt(n)))^2 */

    /* Return gcd(n, sq).  Cast n to seq_t for the computation. */
    seq_t nn = (seq_t)n;
    return my_gcd(nn, sq);
}

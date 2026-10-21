/*
 * A065803  a(n) = (sigma_2(n) mod 2) * (sigma_2(n) mod 5). Residue-product
 *          modulo 2 and 5 of sum of square of divisors.
 * https://oeis.org/A065803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sigma_2(n) mod 2) * (sigma_2(n) mod 5)
 * Range:     u8 n <= 9074, u16 n <= 9074, u32 n <= 9074, u64 n <= 9074,
 *            u128 n <= 9074, big any n
 * Helpers:   sigma2_mod5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/sigma2_mod5.h"

/* Integer square root for 64‑bit values (uses long double for a good start). */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t x = (uint64_t)sqrt((long double)n);
    while ((unsigned __int128)x * x > n) --x;
    while ((unsigned __int128)(x + 1) * (x + 1) <= n) ++x;
    return x;
}

/* Check whether x is a perfect square. */
static bool is_perfect_square_uint64(uint64_t x)
{
    uint64_t r = isqrt_uint64(x);
    return (unsigned __int128)r * r == x;
}

/* sigma_2(n) mod 2 : 1 iff the odd part of n is a perfect square. */
static inline int sigma2_mod2(uint64_t n)
{
    /* strip all factors of 2 */
    while ((n & 1ULL) == 0) n >>= 1;
    return is_perfect_square_uint64(n) ? 1 : 0;
}

/* External function as required by the build system. */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    int r2 = sigma2_mod2(N);
    int r5 = sigma2_mod5(N);
    return (seq_t)(r2 * r5);
}

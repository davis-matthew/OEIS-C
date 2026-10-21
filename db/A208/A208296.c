/*
 * A208296  Smallest positive nontrivial odd solution of the congruence x^2 ==
 *          1 (mod A001748(n+2)), n >= 1.
 * https://oeis.org/A208296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(8*T(K(n))+1) where T(x) = x*(x+1)/2 and K(n) =
 *            prime(n+2)-1 if prime(n+2) ≡ 1 (mod 6) prime(n+2) otherwise This
 *            function returns the smallest positive non‑trivial odd solution
 *            of x^2 ≡ 1 (mod A001748(n+2))
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k); */

/* Integer square root for 64‑bit values without overflow. */
static inline unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = 1ull << 32;   /* 4·10^10 < 2^36 < (1<<32)^2 */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n ≥ 1, but no explicit check required */
    uint32_t p = get_prime(n + 2);            /* prime(n+2) */
    uint32_t K = (p % 6 == 1) ? (p - 1) : p;  /* K(n)        */

    unsigned long long k = K;
    /* 8*T(K)+1 = 4*K*(K+1)+1 */
    unsigned long long val = 4ULL * k * (k + 1) + 1;

    unsigned long long root = isqrt_u64(val);
    return (seq_t)root;
}

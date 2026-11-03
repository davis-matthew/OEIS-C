/*
 * A065803  a(n) = (sigma_2(n) mod 2) * (sigma_2(n) mod 5). Residue-product
 *          modulo 2 and 5 of sum of square of divisors.
 * https://oeis.org/A065803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sigma_2(n) mod 2) * (sigma_2(n) mod 5)
 * Range:     u8 n <= 101761, u16 n <= 101761, u32 n <= 101761,
 *            u64 n <= 101761, u128 n <= 101761, big any n
 * Validated: standard, pass, 137 terms through n = 9074 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* sigma_2(n) mod 5 via prime factorisation. */
static int sigma2_mod5(uint64_t n)
{
    int result = 1;          /* multiplicative identity modulo 5 */
    uint64_t rem = n;

    /* Handle factor 2 separately (only to keep the loop simple). */
    if ((rem & 1ULL) == 0) {
        int e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        /* sum_{k=0..e} 2^{2k} mod 5.  2^2 = 4 ≡ -1 (mod 5). */
        int sum = 0, term = 1;
        for (int k = 0; k <= e; ++k) {
            sum = (sum + term) % 5;
            term = (term * 4) % 5;            /* multiply by 4 ≡ -1 */
        }
        result = (int)((long long)result * sum % 5);
    }

    /* Handle odd prime factors. */
    for (uint64_t p = 3; p * p <= rem; p += 2) {
        if (rem % p != 0) continue;
        int e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }
        /* sum_{k=0..e} p^{2k} mod 5. */
        int p2 = (int)(((p % 5) * (p % 5)) % 5); /* p^2 mod 5 */
        int sum = 0, term = 1;
        for (int k = 0; k <= e; ++k) {
            sum = (sum + term) % 5;
            term = (int)((long long)term * p2 % 5);
        }
        result = (int)((long long)result * sum % 5);
    }

    /* If anything left, it's a prime > sqrt(original n). */
    if (rem > 1) {
        int p = (int)(rem % 5);
        int p2 = (int)(((long long)p * p) % 5);   /* p^2 mod 5 */
        int sum = (1 + p2) % 5;   /* e = 1: term 1 + p^2 */
        result = (int)((long long)result * sum % 5);
    }

    return result;
}

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

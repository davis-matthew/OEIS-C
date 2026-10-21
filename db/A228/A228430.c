/*
 * A228430  Number of ways to write n = x + y (x, y > 0) with x^4 + y*(y+1)/2
 *          prime.
 * https://oeis.org/A228430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) : x>0, y>0, x+y=n, x^4 + y*(y+1)/2 is prime}
 * Range:     u8 n <= 2646, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;

    /* Small prime trial division */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Witnesses for 64‑bit integers (deterministic) */
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0ULL) continue;

        __uint128_t x = 1;
        __uint128_t base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        uint64_t x_val = (uint64_t)x;
        if (x_val == 1 || x_val == n - 1) continue;

        bool cont = false;
        for (unsigned int r = 1; r < s; ++r) {
            x = (x * x) % n;
            x_val = (uint64_t)x;
            if (x_val == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 2U) return (seq_t)0;
    seq_t count = 0;
    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;
        __uint128_t xu = x;
        __uint128_t x_sq = xu * xu;
        __uint128_t x_four = x_sq * x_sq;
        __uint128_t y_mul = (__uint128_t)y * (y + 1) / 2;
        __uint128_t val128 = x_four + y_mul;
        uint64_t val = (uint64_t)val128;           /* fits in 64‑bit for n ≤ 10 000 */
        if (is_prime_uint64(val)) ++count;
    }
    return count;
}

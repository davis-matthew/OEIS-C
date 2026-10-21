/*
 * A055390  Terms of A007634 where n - 40 is not a square.
 * https://oeis.org/A055390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that n^2 + n + 41 is composite and n - 40 is not
 *            a perfect square
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* 64‐bit modular multiplication avoiding overflow */
static uint64_t mul_mod_64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* 64‐bit modular exponentiation */
static uint64_t pow_mod_64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod_64(result, base, mod);
        base = mul_mod_64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64-bit integers. */
static int is_prime_64(uint64_t n) {
    if (n < 2)          return 0;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    if (n % 5 == 0)     return n == 5;

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = pow_mod_64(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;
        int composite = 1;
        for (int r = 1; r < s; ++r) {
            x = mul_mod_64(x, x, n);
            if (x == n - 1ULL) { composite = 0; break; }
        }
        if (composite) return 0;   /* composite */
    }
    return 1;   /* prime */
}

/* Integer square root (floor) for 64-bit unsigned integers */
static uint64_t isqrt_64(uint64_t n) {
    uint64_t lo = 0, hi = (1ULL << 32) - 1, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        uint64_t sq  = (uint64_t)((__uint128_t)mid * mid);
        if (sq <= n) { ans = mid; lo = mid + 1; }
        else         hi = mid - 1;
    }
    return ans;
}

/* Checks whether x is a perfect square */
static int is_square(uint64_t x) {
    uint64_t r = isqrt_64(x);
    return r * r == x;
}

/* Main sequence generator. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    idx_t count = 0;
    /* We look for the n-th integer m satisfying the conditions. */
    for (uint64_t m = 0; ; ++m) {
        /* Polynomial value: m^2 + m + 41 */
        __uint128_t poly128 = (__uint128_t)m * m + m + 41;
        uint64_t poly = (uint64_t)poly128;   /* fits into 64 bits for tested range */

        if (!is_prime_64(poly)) {               /* composite polynomial */
            if (m < 40) {                      /* difference negative – not a square */
                ++count;
                if (count == n) return (seq_t)m;
            } else {                            /* m >= 40 */
                uint64_t diff = m - 40;
                if (!is_square(diff)) {         /* difference not a perfect square */
                    ++count;
                    if (count == n) return (seq_t)m;
                }
            }
        }
    }
}

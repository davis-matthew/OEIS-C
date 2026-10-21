/*
 * A228000  Smallest prime factor of 144396166620968*n + 1.
 * https://oeis.org/A228000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest prime factor of 144396166620968*n + 1 */

#include <stdint.h>
#include <stddef.h>

/* modular arithmetic helpers for 64‑bit operands */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Euclidean gcd for 64‑bit values */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static inline int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases that are sufficient for all 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int composite = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = 0; break; }
        }
        if (composite) return 0;
    }
    return 1;
}

/* Pollard–Rho algorithm for a factor (non‑trivial) of n */
static uint64_t pollard_rho(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_uint64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try a new constant if cycle found */
    }
}

/* recursively find the smallest prime factor of n (n > 1) */
static uint64_t min_prime_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    if (is_prime_uint64(n)) return n;

    uint64_t factor = pollard_rho(n);
    uint64_t a = min_prime_factor(factor);
    uint64_t b = min_prime_factor(n / factor);
    return a < b ? a : b;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* constant 144396166620968 fits in 64 bits */
    const uint64_t C = 144396166620968ULL;

    /* compute m = 144396166620968*n + 1, using 128‑bit to avoid overflow */
    __uint128_t m128 = (__uint128_t)C * (uint64_t) n + 1ULL;
    uint64_t m = (uint64_t) m128;   /* m < 2^63 for n <= 10000 */

    uint64_t spf = min_prime_factor(m);
    return (seq_t) spf;
}

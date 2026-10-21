/*
 * A071256  Smallest multiple of n sandwiched between twin primes.
 * https://oeis.org/A071256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = smallest integer m = n*k such that m-1 and m+1 are twin
 *            primes (i.e. m = p+1 where (p,p+2) are prime). Thus m = k*n with
 *            k*n-1 and k*n+1 prime
 * Range:     u8 n <= 12, u16 n <= 316, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Modulo multiplication using 128-bit intermediate to avoid overflow. */
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* Deterministic Miller‑Rabin test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    /* Check small primes first. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n-1 as d*2^s. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;

        /* Compute a^d mod n via exponentiation by squaring */
        uint64_t x = 1;
        uint64_t pow = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL)
                x = mod_mul64(x, pow, n);
            pow = mod_mul64(pow, pow, n);
            exp >>= 1;
        }

        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t early for multiplication. */
    seq_t n_seq = (seq_t)n;

    /* k starts at 1; p = k*n - 1; check twin primes. */
    for (uint64_t k = 1; ; ++k) {
        /* Compute p = k*n - 1 as 128‑bit to detect overflow. */
        __uint128_t prod = (__uint128_t)k * (__uint128_t)n_seq;
        if (prod > UINT64_MAX)
            return (seq_t)0; /* overflow case – not expected for the test range */

        uint64_t p = (uint64_t)prod - 1ULL;      /* p = k*n - 1 */
        uint64_t p_plus2 = p + 2ULL;

        if (is_prime_u64(p) && is_prime_u64(p_plus2)) {
            /* m = k*n = p+1 */
            return (seq_t)prod;
        }
    }
    /* unreachable */
}

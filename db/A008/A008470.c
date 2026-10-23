/*
 * A008470  At least 3 out of 10m+1, 10m+3, 10m+7, 10m+9 are primes.
 * https://oeis.org/A008470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1653, u32 n <= 8633, u64 n <= 8633,
 *            u128 n <= 8633, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008470: m such that at least 3 of 10m+1,10m+3,10m+7,10m+9 are primes.
   The function scans decades m ≡ 1 (mod 3) and counts prime endpoints
   using a deterministic 64‑bit Miller–Rabin test. */

/* --- required headers ---------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --- 64‑bit deterministic Miller–Rabin ----------------------------------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1U) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1U) == 0U) { d >>= 1U; ++s; }
    /* Deterministic bases for n < 2^64 */
    static const uint64_t test_bases[] = {
        2,325,9375,28178,450775,9780504,1795265022
    };
    for (size_t i = 0; i < sizeof(test_bases)/sizeof(test_bases[0]); ++i) {
        uint64_t a = test_bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1U || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1U; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based; count how many qualifying decades have been found */
    idx_t count = 0;
    /* start with the first decade congruent to 1 mod 3 */
    idx_t m = 1;

    while (count < n) {
        /* compute the four 10m+{1,3,7,9} values */
        uint64_t v1 = 10ULL * m + 1;
        uint64_t v3 = 10ULL * m + 3;
        uint64_t v7 = 10ULL * m + 7;
        uint64_t v9 = 10ULL * m + 9;

        int primes = 0;
        if (is_prime_u64(v1)) ++primes;
        if (is_prime_u64(v3)) ++primes;
        if (is_prime_u64(v7)) ++primes;
        if (is_prime_u64(v9)) ++primes;

        if (primes >= 3) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        /* advance to next decade ≡ 1 (mod 3) */
        m += 3;
    }
    /* unreachable for valid n, but keeps the compiler happy */
    return (seq_t)0;
}

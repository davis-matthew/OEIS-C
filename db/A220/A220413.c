/*
 * A220413  Number of ways to write n=x+y (x>=0, y>=0) with x^3+2*y^3 prime.
 * https://oeis.org/A220413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of partitions n = x + y (x≥0, y≥0) for which
   x^3 + 2*y^3 is prime.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* 64‑bit deterministic Miller–Rabin primality test */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t power  = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = modmul64(result, power, mod);
        power = modmul64(power, power, mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) return true;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* External function required by the build system */
seq_t a(idx_t n)
{
    /* Offset is 1; for n=0 return 0 safely */
    if (n == 0) return 0;

    seq_t count = 0;
    for (idx_t x = 0; x <= n; ++x) {
        idx_t y = n - x;
        unsigned __int128 xx = (unsigned __int128)x;
        unsigned __int128 yy = (unsigned __int128)y;
        unsigned __int128 val128 = xx*xx*xx + (unsigned __int128)2*yy*yy*yy;
        uint64_t val = (uint64_t)val128;   /* n ≤ 10000 ⇒ val < 3·10^12 */
        if (isPrime64(val))
            ++count;
    }
    return count;
}

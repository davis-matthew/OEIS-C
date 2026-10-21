/*
 * A083803  Smallest number k such that the concatenations k 2n-1 and 2n-1 k
 *          are both primes, or 0 if n = 5m+3.
 * https://oeis.org/A083803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k such that the concatenations k‖(2n−1) and
 *            (2n−1)‖k are both prime; a(n) = 0 when n ≡ 3 (mod 5)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit deterministic Miller–Rabin primality test */
static inline bool isPrime(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallPrimes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        if (n == smallPrimes[i]) return true;
        if (n % smallPrimes[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        uint64_t x = 1, p = a, e = d;
        while (e) {
            if (e & 1) x = (unsigned __int128)x * p % n;
            p = (unsigned __int128)p * p % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* decimal digit count for 64‑bit unsigned integer */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* 10^exp for exp up to 19 (fits in 64‑bit) */
static inline uint64_t pow10_uint(int exp)
{
    uint64_t r = 1;
    while (exp--) r *= 10ULL;
    return r;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n % 5 == 3) return (seq_t)0;   /* special case */

    uint64_t m = 2ULL * n - 1;                 /* odd number 2n-1 */
    int dm = digits_uint64(m);
    uint64_t pow10_dm = pow10_uint(dm);

    for (uint64_t k = 1; ; ++k) {
        uint64_t a1 = (__uint128_t)k * pow10_dm + m;
        if (!isPrime(a1)) continue;

        int dk = digits_uint64(k);
        uint64_t pow10_dk = pow10_uint(dk);
        uint64_t a2 = (__uint128_t)m * pow10_dk + k;
        if (isPrime(a2)) return (seq_t)k;
    }
}

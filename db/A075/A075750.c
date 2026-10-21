/*
 * A075750  210*n -/+ 11 are primes.
 * https://oeis.org/A075750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A075750 – numbers n such that 210*n‑11 and 210*n+11 are both prime. */
/* Computation: n is counted from 1; a(1)=2, a(2)=3, … etc. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n‑1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases that are deterministic for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* compute a^d mod n */
        __uint128_t x = 1;
        __uint128_t p = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }
        uint64_t xi = (uint64_t)x;
        if (xi == 1 || xi == n - 1) continue;

        bool cont = false;
        for (int r = 1; r < s; ++r) {
            xi = (__uint128_t)xi * xi % n;
            if (xi == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Sequence generator: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0; guard against under‑flow */

    uint64_t target   = (uint64_t)n;
    uint64_t count    = 0;
    uint64_t candidate = 2;        /* start from 2, as 1 never satisfies the condition */

    while (true) {
        /* skip multiples of 11 – both numbers would then be divisible by 11 */
        if (candidate % 11ULL != 0ULL) {
            uint64_t left  = 210ULL * candidate - 11ULL;
            uint64_t right = 210ULL * candidate + 11ULL;
            if (is_prime_u64(left) && is_prime_u64(right)) {
                ++count;
                if (count == target) return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}

/*
 * A104200  Upper bound on a straddle prime pair.
 * https://oeis.org/A104200
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS formula (informal description):
 * a(n) is the smallest prime greater than the (n-3)-th composite number.
 * The composite numbers start 4,6,8,9,10,...  (offset 4).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* 64‑bit deterministic Miller–Rabin primality test */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }

    /* bases sufficient for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* modular exponentiation */
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        uint64_t xr = (uint64_t)x;
        if (xr == 1 || xr == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            xr = (uint64_t)x;
            if (xr == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* a(n): upper bound on a straddle prime pair */
seq_t a(idx_t n)
{
    if (n < 4) return 0;  /* undefined below offset; returning 0 */

    /* determine the (n-3)-th composite number */
    idx_t target = n - 3;          /* 1‑based composite index */
    uint64_t composite = 4;        /* first composite */
    idx_t count = 1;               /* we've seen one composite */

    while (count < target) {
        ++composite;
        if (!isPrime64(composite)) ++count;
    }

    /* find the smallest prime greater than this composite */
    uint64_t prime = composite + 1;
    while (!isPrime64(prime)) ++prime;

    return (seq_t)prime;
}

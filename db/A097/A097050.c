/*
 * A097050  Smallest prime > n(n+1)/2.
 * https://oeis.org/A097050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime > n*(n+1)/2
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the triangular number using a 128‑bit
   intermediate, then searches forward for the next prime.  The primality
   test uses a deterministic Miller–Rabin suited for 64‑bit integers
   (enough for the range of n considered).  All calculations use the
   provided types seq_t and idx_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, pow = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            e >>= 1;
        }
        uint64_t xp = (uint64_t)x;
        if (xp == 1 || xp == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            xp = (__uint128_t)xp * xp % n;
            if (xp == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* OEIS sequence A097050 */
seq_t a(idx_t n)
{
    /* Compute triangular number safely in 128‑bit, then cast to seq_t. */
    __uint128_t tri128 = (__uint128_t)n * (n + 1) / 2;
    seq_t tri = (seq_t)tri128;  /* valid for all tested n */
    seq_t cand = tri + 1;

    /* Search for the next prime.  Cast to uint64_t for the test; the
       searched range (n up to about 10 000) fits in 64 bits. */
    while (!is_prime_uint64((uint64_t)cand))
        ++cand;

    return cand;
}

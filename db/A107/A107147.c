/*
 * A107147  Primes of the form 3x^2 + 14y^2.
 * https://oeis.org/A107147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modmul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 3*x^2 + 14*y^2.  These primes are exactly the
   primes congruent to {3,17,41,59,83,89,131} modulo 168.  
   The function scans candidate numbers in that residue set and
   tests them for primality. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/modmul64.h"

/* ---------- primality test for uint64_t ----------
   Deterministic Miller–Rabin using the 7 bases that work for
   all 64‑bit unsigned integers.  */

static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t res = 1 % m;
    a %= m;
    while (e) {
        if (e & 1)
            res = modmul64(res, a, m);
        a = modmul64(a, a, m);
        e >>= 1;
    }
    return res;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* check divisibility by small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64-bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned int r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1 */

    /* residues modulo 168 that can contain the required primes */
    static const uint64_t residues[7] = {
        3ULL, 17ULL, 41ULL, 59ULL, 83ULL, 89ULL, 131ULL
    };

    idx_t count = 0;
    /* k is the multiplier for the residue 168 */
    for (uint64_t k = 0; ; ++k) {
        for (size_t r = 0; r < sizeof(residues)/sizeof(residues[0]); ++r) {
            uint64_t candidate = residues[r] + 168ULL * k;
            /* 3 is the first term and is already covered by r==0,k==0 */
            if (isPrime64(candidate)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
    }
}

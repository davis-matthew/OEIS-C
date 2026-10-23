/*
 * A107146  Primes of the form 6x^2 + 7y^2.
 * https://oeis.org/A107146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p ≡ 7,13,31,55,61,103,157 (mod 168)
 *            /* (primes of the form 6x² + 7y²). The function generates primes
 *            * in order and keeps only those with one of the seven residues *
 *            modulo 168
 * Range:     u8 n <= 10, u16 n <= 829, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Deterministic Miller–Rabin for 64‑bit integers ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mul_mod(r, a, m);
        a = mul_mod(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
        59,61,67,71,73,79,83,89,97,101,103,107,109,113,
        127,131,137,139,149,151,157,163,167
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,
                                     450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool passed = false;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n-1) { passed = true; break; }
        }
        if (!passed) return false;
    }
    return true;
}

/* ---- main sequence generator ---- */
static const uint8_t residues[7] = {7,13,31,55,61,103,157};

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1 */

    static uint64_t *primes = NULL;
    static size_t cap = 0, len = 0;
    static uint64_t cand = 3;  /* start from first odd prime candidate */

    while (len < (size_t)n)
    {
        if ((cand & 1) == 0) { cand++; continue; }

        if (!is_prime_64(cand))
        {
            cand++;
            continue;
        }

        uint64_t mod = cand % 168;
        bool ok = false;
        for (int i = 0; i < 7; ++i)
            if (mod == residues[i]) { ok = true; break; }

        if (ok)
        {
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes = tmp;
                cap = new_cap;
            }
            primes[len++] = cand;
        }
        cand++;
    }

    return (seq_t)primes[n-1];
}

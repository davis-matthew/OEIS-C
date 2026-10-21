/*
 * A064259  Values of m such that N=(am+1)(bm+1)(cm+1) is a 3-Carmichael
 *          number (A087788), where a,b,c = 1,2,45.
 * https://oeis.org/A064259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow64, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A064259
 *  Sequence of m such that N = (m+1)(2m+1)(45m+1) is a 3‑Carmichael number.
 *  For each term, m ≡ 66 (mod 90) and m+1, 2m+1, 45m+1 are primes.
 *  The function generates terms lazily and caches them for future calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mod_pow64.h"
#include "../utils/mul_mod64.h"

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;           /* 2,3                 */
    static const uint32_t small[] = {
        2,3,5,7,11,13,17,19,23,29,
        31,37,41,43,47,53,59,61,67,71,
        73,79,83,89,97,101,103,107,109,113,
        127,131,137,139,149,151,157,163,167,173,
        179,181,191,193,197,199,211,223,227,229,
        233,239,241,251,257,263,269,271,277,281,
        283,293,307,311,313,317,331,337,347,349,
        353,359,367,373,379,383,389,397,401,409,
        419,421,431,433,439,443,449,457,461,463,
        467,479,487,491,499,503,509,521,523,541
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint32_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2,325,9375,28178,450775,9780504,1795265022
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --- Cache for sequence terms ------------------------------------------------------- */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static uint64_t cache_next_k = 0;   /* next integer k such that m = 66 + 90k */

/* Append new term, assuming cache has enough space */
static void append_new_term(seq_t val)
{
    if (cache_len < MAX_CACHE)
    {
        cache[cache_len++] = val;
    }
}

/* Generate terms until the cache contains at least n elements */
static void generate_terms(idx_t n)
{
    while ((idx_t)cache_len < n)
    {
        uint64_t m = 66 + 90ull * cache_next_k;
        ++cache_next_k;

        uint64_t p = m + 1;
        uint64_t q = 2 * m + 1;
        uint64_t r = 45 * m + 1;

        if (is_prime_u64(p) && is_prime_u64(q) && is_prime_u64(r))
        {
            append_new_term((seq_t)m);
        }
    }
}

/* --- Sequence function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below the offset */
    generate_terms(n);
    return cache[n-1];
}

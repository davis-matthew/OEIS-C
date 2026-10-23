/*
 * A176130  Lesser of a pair (p,p+4) of cousin primes whose arithmetic mean
 *          p+2 is a square number.
 * https://oeis.org/A176130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lesser prime p of a cousin‑prime pair (p, p+4) for which
 *            the mean p+2 is a perfect square. The pair is parameterised by k
 *            ≥ 0: p = 36 k² + 36 k + 7 (prime) p+4 = 36 k² + 36 k + 11
 *            (prime) a(n) is the n‑th such p in increasing order
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit deterministic Miller–Rabin  ---------- */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes filter */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n–1 as d·2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for all 64‑bit numbers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* modular exponentiation a^d mod n using 128‑bit multiplication */
        uint64_t x = 1, base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1ULL)
                x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  sequence implementation  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range guard */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t  next_k      = 0; /* next k to test */

    /* expand cache until we have n terms */
    while (cache_size < n) {
        uint64_t k = (uint64_t)next_k;
        uint64_t p  = 36ULL * k * k + 36ULL * k + 7ULL;     /* p   = 36k^2 + 36k + 7  */
        uint64_t p4 = p + 4ULL;                             /* p+4 = 36k^2 + 36k + 11 */

        if (is_prime_64(p) && is_prime_64(p4)) {
            /* grow cache if needed */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;          /* allocation failure – unlikely */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)p;
        }
        ++next_k;
    }

    return cache[n - 1];
}

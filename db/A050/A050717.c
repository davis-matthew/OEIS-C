/*
 * A050717  Inserting a digit '7' between adjacent digits of n makes a prime.
 * https://oeis.org/A050717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer n such that inserting digit 7 between
 *            every pair of adjacent decimal digits of n yields a prime
 * Range:     u8 n <= 2, u16 n <= 857, u32 n <= 6495, u64 n <= 9267,
 *            u128 n <= 9267, big any n
 * Helpers:   insert_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/insert_sevens.h"

static seq_t mul_mod64(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    seq_t res = 0, base = a % mod;
    while (b) {
        if (b & 1) res = (res + base) % mod;
        base = (base + base) % mod;
        b >>= 1;
    }
    return res;
#endif
}

/* Deterministic Miller‑Rabin for 64‑bit unsigned integers. */
static bool is_prime(seq_t n)
{
    if (n < 2) return false;

    /* Small‑prime trial */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
        17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        seq_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller–Rabin */
    seq_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        seq_t a = bases[i] % n;
        if (a == 0) continue;

        seq_t x = a, r = 1;
        seq_t exp = d;
        while (exp) {
            if (exp & 1) r = mul_mod64(r, x, n);
            x = mul_mod64(x, x, n);
            exp >>= 1;
        }
        if (r == 1 || r == n - 1) continue;

        bool composite = true;
        for (uint32_t j = 1; j < s; ++j) {
            r = mul_mod64(r, r, n);
            if (r == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Sequence function, external linkage. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static uint64_t next_candidate = 13;  /* first term from OEIS list */

    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t ins  = insert_sevens(cand);

        if (is_prime(ins)) {
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len] = cand;
            ++cache_len;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

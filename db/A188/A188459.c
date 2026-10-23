/*
 * A188459  Numbers k such that 4*k^2 + 4*k + 653 is a prime.
 * https://oeis.org/A188459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 160, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k such that 4*k^2 + 4*k + 653 is prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Miller–Rabin deterministic test for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that guarantee determinism for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        /* modular exponentiation */
        uint64_t x = 1;
        uint64_t pow = d;
        uint64_t base = a;
        while (pow) {
            if (pow & 1ULL) {
                __uint128_t prod = ( __uint128_t )x * base % n;
                x = (uint64_t)prod;
            }
            pow >>= 1;
            __uint128_t prod = ( __uint128_t )base * base % n;
            base = (uint64_t)prod;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            __uint128_t prod = ( __uint128_t )x * x % n;
            x = (uint64_t)prod;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of sequence values */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_k = 0;   /* smallest k not yet tested */

    while (cache_len < (size_t)n) {
        uint64_t k = next_k;
        /* compute 4*k^2 + 4*k + 653 safely in 128‑bit then cast to 64‑bit */
        unsigned __int128 expr128 = (unsigned __int128)4 * k * k
                                + (unsigned __int128)4 * k
                                + 653ULL;
        uint64_t expr = (uint64_t)expr128;
        if (is_prime_uint64(expr)) {
            if (cache_len >= cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* out of memory */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return cache[n-1];
}

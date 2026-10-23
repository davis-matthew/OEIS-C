/*
 * A108106  Numbers n such that the string 1119n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A108106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108106: a(n) = n‑th integer m such that the decimal concatenation
   1119‖m (i.e. 1119 * 10^d + m, where d = number of decimal digits in m)
   is a prime number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ******************************
   64‑bit deterministic Miller–Rabin
   ****************************** */
static bool is_prime64(uint64_t n)
{
    if (n < 2)                    return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL,
         450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a;
        uint64_t e = d;
        while (e) {
            if (e & 1) x = (x * p) % n;
            p = (p * p) % n;
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Static cache of found terms */
static seq_t *cache = NULL;
static size_t cache_sz = 0, cache_cap = 0;

/* State for incremental search */
static uint64_t next_candidate = 1;   /* next m to test */
static unsigned long long pow10 = 10ULL;   /* 10^d for current digit count d */
static unsigned d = 1;                   /* current digit count */

/* Append value to cache, grow if needed */
static void add_term(seq_t val)
{
    if (cache_sz == cache_cap) {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_sz++] = val;
}

/* Compute a(n) with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe */

    while (cache_sz < (size_t)n) {
        /* move to next digit length when necessary */
        if (next_candidate == pow10) {
            ++d;
            pow10 *= 10ULL;
        }

        /* form concatenated number: 1119*10^d + next_candidate */
        uint64_t concat = 1119ULL * pow10 + next_candidate;
        if (is_prime64(concat)) add_term((seq_t)next_candidate);

        ++next_candidate;
    }

    return cache[n - 1];
}

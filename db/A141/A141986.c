/*
 * A141986  Primes congruent to 10 mod 29.
 * https://oeis.org/A141986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 10 modulo 29.
   a(1) = 97, a(2) = 271, … */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Static cache of found primes that are 10 mod 29.
       The cache holds 64‑bit values; they are converted to seq_t when returned. */
    static uint64_t *cache = NULL;
    static idx_t  count  = 0;      /* number of cached values */
    static size_t cap    = 0;      /* allocated capacity of cache */
    static uint64_t next_k = 3;    /* k such that 29*k+10 is the next candidate */

    /* Grow the cache until we have at least n primes. */
    while (count < n) {
        uint64_t cand = 29 * next_k + 10;   /* candidate ≡ 10 (mod 29) */
        if (is_prime_big((seq_t)cand)) {
            /* Allocate or grow the cache. */
            if (count == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                uint64_t *new_cache = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
                if (!new_cache) {
                    /* Out of memory – abort. */
                    exit(EXIT_FAILURE);
                }
                /* Copy existing entries. */
                for (idx_t i = 0; i < count; ++i) {
                    new_cache[i] = cache[i];
                }
                /* Free old storage. */
                free(cache);
                cache = new_cache;
                cap   = new_cap;
            }
            cache[count++] = cand;
        }
        ++next_k;
    }
    return (seq_t)cache[n - 1];   /* 0‑based indexing into the cache */
}

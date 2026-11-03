/*
 * A141915  Primes congruent to 11 mod 23.
 * https://oeis.org/A141915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 11 (mod 23)
 * Range:     u8 n <= 4, u16 n <= 303, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence generator for OEIS A141915.
   It uses a simple incremental search with a cache so that calls
   can come in any order.  The built-in primality test
   is_prime_big is available in the utilities and works for all
   supported integer types. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"
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
    /* static cache of primes found so far */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;
    static seq_t  next_candidate;
    static bool   init       = false;

    /* one-time initialization */
    if (!init) {
        init = true;
        cache_cap          = 64;
        cache              = mem_new(seq_t, cache_cap);
        next_candidate     = (seq_t)11;   /* first candidate 11 ≡ 11 (mod 23) */
    }

    /* generate primes until the cache contains at least n primes */
    while (cache_len < n) {
        seq_t cand = next_candidate;

        /* test for primality */
        if (is_prime_big(cand)) {
            /* store it in the cache */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = cand;
        }

        /* advance to the next number ≡ 11 (mod 23) */
        next_candidate = cand + (seq_t)23;
    }

    return cache[n - 1];
}

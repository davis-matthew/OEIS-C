/*
 * A141945  Primes congruent to 23 mod 25.
 * https://oeis.org/A141945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 329, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of prime numbers whose last two decimal digits
   are 23 or 73.  We generate primes of the form p = 25·k + 23
   (congruence 23 (mod 25)) and return the n‑th such prime. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* Utility for primality testing of arbitrary‑size integers */
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

/* Return the n‑th prime p such that p % 25 == 23 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of previously found primes */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;
    static idx_t  last_k    = 0;     /* k value of the last tested candidate */
    
    /* Allocate initial cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        cache_sz = 0;
        last_k = 0;
    }

    /* If the requested index is already cached, return it */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Generate further primes until we reach the desired index */
    while (cache_sz < n) {
        seq_t cand = (seq_t)last_k * 25 + 23;
        if (is_prime_big(cand)) {
            /* Grow cache if needed */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (new_cache == NULL) /* allocation failure; abort */
                    return (seq_t)0;
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = cand;
        }
        last_k++;
    }
    return cache[n - 1];
}

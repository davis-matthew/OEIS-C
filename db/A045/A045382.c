/*
 * A045382  Primes congruent to {2, 7} mod 8.
 * https://oeis.org/A045382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1650, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9935 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 2 or 7 modulo 8
   (sequence A045382).  Uses incremental primality testing
   and an expanding static cache of the primes found so far. */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;      /* cache of found primes          */
static size_t cache_cap = 0;     /* capacity of the cache          */
static size_t cached_count = 0;  /* number of primes stored        */
static seq_t current = 2;        /* last candidate examined */
static bool init = false;        /* whether cache has been primed */

static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* permutation is 1‑based; 0 is unused */

    if (!init) {            /* initialise with the first prime 2     */
        ensure_cache(1);
        cache[0] = 2;
        cached_count = 1;
        current = 2;        /* last prime already seen */
        init = true;
    }

    /* Generate primes until we have at least n of them */
    while (cached_count < (size_t)n) {
        /* jump to the next integer that is 2 or 7 mod 8 */
        do {
            current++;
        } while ((current % 8 != 2) && (current % 8 != 7));

        if (is_prime_big(current)) {
            ensure_cache(cached_count + 1);
            cache[cached_count++] = current;
        } else {
            /* not prime – continue looping to find the next candidate */
            continue;
        }
    }

    return cache[(size_t)(n - 1)];
}

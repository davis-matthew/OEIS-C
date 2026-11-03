/*
 * A142780  Primes congruent to 53 mod 59.
 * https://oeis.org/A142780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 53 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 933 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A142780)  Offset: 1 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>            /* for memcpy */

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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* static cache of found primes */
static seq_t *cache = NULL;      /* array of primes */
static size_t cache_size = 0;    /* number of primes stored */
static size_t cache_cap   = 0;    /* allocated capacity       */
static seq_t  candidate   = 53;   /* next number ≡ 53 (mod 59) */
static bool   initialized = false;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    /* grow to at least needed, doubling until enough */
    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = seq_alloc_array(new_cap);
    if (!tmp) {
        /* allocation failure – terminate safely */
        cache_size = 0;
        cache_cap  = 0;
        cache = NULL;
        return;
    }
    /* copy existing primes */
    if (cache && cache_size)
        mem_copy(tmp, cache, cache_size * sizeof(seq_t));
    /* free old array */
    if (cache)
        free_seq_array(cache);
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but guard anyway */

    if (!initialized) {
        /* initial allocation – enough for many primes (≈200 000) */
        cache_cap = 20000;
        cache     = seq_alloc_array(cache_cap);
        cache_size = 0;
        candidate  = 53;      /* start from the first possible prime */
        initialized = true;
    }

    /* grow cache if required */
    ensure_capacity(n);

    /* extend cache until it contains the nth prime */
    while (cache_size < (size_t)n) {
        if (is_prime_big(candidate)) {
            cache[cache_size++] = candidate;
        }
        candidate += 59;        /* next number ≡53 (mod 59) */
    }

    return cache[(size_t)n - 1];
}

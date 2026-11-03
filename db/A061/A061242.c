/*
 * A061242  Primes of the form 9*k - 1.
 * https://oeis.org/A061242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 990 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterable generating primes of the form 9*k-1 (congruent to 8 mod 9). */
/* The function memoises results so calls may come in any order. */
/* The code uses the provided is_prime_big() routine for primality testing. */

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

static seq_t *cache = NULL;          /* cached primes */
static size_t cache_cap = 0, cache_len = 0;  /* capacity and current length */
static idx_t next_k = 2;             /* next k to test (9*k-1) */

/* Extend cache until it holds at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        /* Grow the cache if needed. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
            if (!new_cache)
                abort();              /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Candidate of the form 9*k-1. */
        idx_t candidate_idx = (idx_t)9 * next_k - (idx_t)1;
        seq_t candidate = (seq_t)candidate_idx;   /* wrap if too large for seq_t */

        /* Test for primality. */
        if (is_prime_big(candidate))
            cache[cache_len++] = candidate;

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but safe guard */
    ensure_cache(n);
    return cache[n - 1];
}

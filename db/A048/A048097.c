/*
 * A048097  Numbers k such that k^2 + k + 11 is prime.
 * https://oeis.org/A048097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   k such that k^2 + k + 11 is prime
 * Range:     u8 n <= 13, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 898 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) returns the nth such k, with a(1) = 0) */

#include <stddef.h>
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

/* Cache for already found terms */
static seq_t *cache      = NULL;   /* array of k-values */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_size = 0;      /* number of valid entries */
static idx_t  next_k     = 0;      /* next candidate k to test */

/* Ensure there is room for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need)
            new_cap *= 2;

        /* Allocate new block */
        seq_t *new_buf = mem_new(seq_t, new_cap);
        if (!new_buf)
            abort();

        /* Copy existing elements */
        for (size_t i = 0; i < cache_size; ++i)
            new_buf[i] = cache[i];

        mem_free(cache);
        cache = new_buf;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification; behaviour for n < 1 is irrelevant */
    if (n == 0)
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* Compute k^2 + k + 11 */
        seq_t kk  = (seq_t)next_k;
        seq_t val = kk * kk + kk + (seq_t)11;

        /* Test primality */
        if (is_prime_big(val))
        {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}

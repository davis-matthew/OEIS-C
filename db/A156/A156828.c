/*
 * A156828  a(1) = 2. a(n) = the smallest prime >= a(n-1) + 4.
 * https://oeis.org/A156828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = the smallest prime ≥ a(n-1)+4 for n>1
 * Range:     u8 n <= 37, u16 n <= 5682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9488 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* realloc, abort */
#include <stdbool.h>     /* bool, false, true */
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

/* Cached values of the sequence.  cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of filled entries */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure cache contains at least n entries (a(1)…a(n)) */
static void ensure_cache(idx_t n)
{
    /* nothing to do for invalid indices */
    if (n < 1)
        return;

    /* allocate or grow the array */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();           /* unrecoverable failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;          /* a(1) = 2 */
        cache_len = 1;
    }
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t candidate = cache[i - 1] + (seq_t)4;
        while (!is_prime_big(candidate))
            candidate++;
        cache[i] = candidate;
    }
    cache_len = (size_t)n;
}

/* Return the nth term of the sequence.  The function is safe to call
   in any order of n.  For n<=0 it simply returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}

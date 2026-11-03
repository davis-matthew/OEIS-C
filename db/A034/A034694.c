/*
 * A034694  Smallest prime == 1 (mod n).
 * https://oeis.org/A034694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = the smallest prime p with p ≡ 1 (mod n), i.e. p = k*n + 1
 *            with k > 0 and p prime
 * Range:     u8 n <= 30, u16 n <= 1690, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9062 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t *cache = NULL;      /* cache[n] = a(n); cache[0] unused */
static size_t cache_size = 0;    /* allocated cache size (>= max n + 1) */

/* Ensure that cache can hold indices up to n. */
static void ensure_cache_size(idx_t n)
{
    /* In OEIS indexing, n ≥ 1. n+1 guarantees space for index n. */
    if ((size_t)n + 1 <= cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* C++: allocate a new array and copy old values. */
    new_cache = new seq_t[new_size];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    /* C: use realloc and initialize new elements to 0. */
    new_cache = (seq_t *)realloc(cache, new_size * sizeof(*new_cache));
    if (!new_cache) {
        /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
#endif

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0, but protect against it */

    ensure_cache_size(n);

    if (cache[n] != 0)
        return cache[n];

    seq_t N = (seq_t)n;
    idx_t k = 1;
    while (1) {
        seq_t p = N * (seq_t)k + 1;   /* candidate prime */
        if (is_prime_big(p)) {
            cache[n] = p;
            return p;
        }
        ++k;
    }
}

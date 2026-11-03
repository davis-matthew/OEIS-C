/*
 * A179445  Nonprimes m such that 10m + 1 are primes.
 * https://oeis.org/A179445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonprime m such that 10m + 1 are primes. */
/* Computed by scanning m from 1 upwards and testing primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"
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

/* Static cache of computed terms. */
static seq_t *cache = NULL;          /* array of seen terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* capacity of the array */
static idx_t last_m = 0;             /* last m that has been examined */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) {
        /* In a library context, aborting is acceptable. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (cache && cache_size >= n)
        return cache[n - 1];

    /* Generate more terms until we have at least n of them. */
    while (cache_size < n) {
        last_m++;                     /* Next candidate m. */

        /* Skip prime m; we need non‑prime m (including 1). */
        if (is_prime(last_m))
            continue;

        /* Compute 10*m + 1. Use uint64_t as an intermediate
           to avoid overflow for moderate m. */
        uint64_t p_val = 10ULL * (uint64_t)last_m + 1ULL;
        seq_t p = (seq_t)p_val;

        /* Test whether 10*m + 1 is prime. */
        if (!is_prime_big(p))
            continue;

        /* Valid term found: store it. */
        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = (seq_t)last_m;
    }

    return cache[n - 1];
}

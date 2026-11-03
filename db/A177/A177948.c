/*
 * A177948  Numbers n such that n^2 + reverse of n^2 is a prime.
 * https://oeis.org/A177948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find numbers n such that n² + reverse(n²) is a prime. */

#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include "../utils/reverse_digits.h"
#include "../utils/is_prime_u64.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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

/* Global cache to store the terms of the sequence. */
static seq_t *cache   = NULL;   /* pointer to cached seq_t values */
static size_t cache_size = 0;    /* number of terms currently stored */
static size_t cache_cap  = 0;    /* allocated capacity of the cache */

/* Ensure the cache has room for at least `need` terms. */
static void
ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;

    if (cache_cap == 0)
        cache = seq_alloc(new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);

    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for indices < 1 */

    /* Expand the cache until it contains the desired term. */
    if (cache_size < (size_t)n) {
        ensure_cache_cap((size_t)n);

        /* Start searching from the smallest integer that has not yet been
           considered.  The first term is 1. */
        idx_t cand = (cache_size == 0) ? 1
                                        : (idx_t)cache[cache_size - 1] + 1;

        for (; cache_size < (size_t)n; ++cand) {
            seq_t sq = (seq_t)cand;
            sq = sq * sq;                    /* n² */

            seq_t rev_sq = reverse_digits(sq);  /* reverse(n²) */
            seq_t sum    = sq + rev_sq;         /* n² + reverse(n²) */

            if (is_prime_big(sum))
                cache[cache_size++] = (seq_t)cand;  /* store the term */
        }
    }

    return cache[(size_t)n - 1];
}

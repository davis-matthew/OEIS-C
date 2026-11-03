/*
 * A228357  Numbers k such that sum of all primes <= k is not prime.
 * https://oeis.org/A228357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n‑th) integer k such that sum of all primes ≤ k is
 *            composite. (OEIS A228357)
 * Range:     u8 n <= 27, u16 n <= 828, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 957 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses a growing cache of computed terms.  It works for
   the usual 8/16/32/64‑bit unsigned integer types employed by the
   reference build, and for the arbitrary‑precision `seq_t` type used
   by the C++ build.  The only required helper functions are
   `is_prime(idx_t)` that tests primality of an idx_t, and
   `is_prime_big(seq_t)` that tests primality of a seq_t. */

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

/* Cache of sequence values.  The cache grows as needed and is
   shared across all calls. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of stored values */

/* The largest k that has been processed so far and the running sum
   of primes ≤ that k. */
static idx_t  last_k      = 0;
static seq_t  sum_primes  = 0;

/* Append a new element to the cache, expanding it if necessary. */
static void append_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behaviour for n < 1 is irrelevant */

    /* Ensure the cache contains at least n values. */
    while (cache_len < (size_t)n) {
        ++last_k;  /* next integer to examine */

        if (is_prime(last_k))
            sum_primes += (seq_t)last_k;

        /* If the running prime sum is composite, store the current k. */
        if (!is_prime_big(sum_primes))
            append_to_cache((seq_t)last_k);
    }

    return cache[(size_t)n - 1];
}

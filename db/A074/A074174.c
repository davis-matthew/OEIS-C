/*
 * A074174  Smallest number k such that k and k+n are of the form p^2*q where
 *          p and q are primes.
 * https://oeis.org/A074174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k and k+n are both of the form p^2*q
 *            where p and q are primes and p != q. The computation searches k
 *            incrementally and memoises results
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* ---------- prime table ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure we have all primes up to at least “limit”. */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit)
        return;

    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* ---------- test whether x is of the form p^2*q with p!=q ---------- */
static bool is_form(seq_t x)
{
    /* The smallest such number is 8 = 2^3, but we disallow p==q. */
    if (x < 8)
        return false;

    /* sqrt(x) limits the possible p. */
    seq_t lim = floor_sqrt(x);
    idx_t sqrt_lim = (idx_t)lim;
    if ((seq_t)sqrt_lim * (seq_t)sqrt_lim < x)
        sqrt_lim++;                      /* round up to be safe */

    ensure_primes(sqrt_lim + 1);

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if ((seq_t)p > sqrt_lim)
            break;
        if (x % (seq_t)p != 0)
            continue;

        seq_t p2 = (seq_t)p * (seq_t)p;
        if (x % p2 != 0)
            continue;

        seq_t m = x / p2;
        if (m <= 1)
            continue;
        if ((seq_t)p == m)               /* p and q must be distinct */
            continue;
        return is_prime_big(m);
    }
    return false;
}

/* ---------- memoisation of a(n) ---------- */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_len)
        return;

    idx_t new_len = n + 1;                /* 1‑based indexing; 0 unused */
    a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
    for (idx_t i = a_cache_len; i < new_len; ++i)
        a_cache[i] = 0;
    a_cache_len = new_len;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n < 1 */
        return 0;

    ensure_a_cache(n);
    if (a_cache[n] != 0)
        return a_cache[n];

    seq_t k = 1;
    for (;;) {
        if (is_form(k) && is_form(k + n)) {
            a_cache[n] = k;
            return k;
        }
        ++k;
    }
}

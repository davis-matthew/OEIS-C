/*
 * A046443  Composite numbers whose 3 prime factors are distinct in length.
 * https://oeis.org/A046443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function yields the composite numbers that are the product of exactly
 * three distinct primes whose decimal lengths are all pairwise distinct.
 * It searches integers and uses a small‑prime‑factor sieve. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"

/* Smallest‑prime‑factor table */
static uint32_t *spf = NULL;
static size_t    spf_cap = 0;
static uint32_t  max_checked = 0;

/* Cache of sequence terms */
static seq_t *cache = NULL;
static size_t  cache_len = 0;
static size_t  cache_cap = 0;

/* Rebuilds the smallest‑prime‑factor array up to 'limit' (inclusive). */
static void build_spf(uint32_t limit)
{
    if (spf_cap < (size_t)limit + 1)
    {
        uint32_t *new_spf = (uint32_t*)realloc(spf, ((size_t)limit + 1) * sizeof(uint32_t));
        if (!new_spf) abort();
        spf = new_spf;
        for (size_t i = 0; i <= (size_t)limit; ++i) spf[i] = 0;
        spf_cap = (size_t)limit;
    }

    size_t primes_sz = (limit >> 4) + 16;
    uint32_t *primes = (uint32_t*)malloc(primes_sz * sizeof(uint32_t));
    if (!primes) abort();
    size_t prime_count = 0;

    for (uint32_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (prime_count < primes_sz) primes[prime_count++] = i;
        }
        for (size_t j = 0; j < prime_count; ++j)
        {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (p > spf[i] || ip > limit) break;
            spf[(uint32_t)ip] = p;
        }
    }
    free(primes);
}

/* Checks whether n = p*q*r with p,q,r distinct primes and distinct
 * decimal lengths. */
static bool is_candidate(uint32_t n)
{
    uint32_t primes[3];
    int count = 0;
    uint32_t last = 0;
    bool repeat = false;

    uint32_t tmp = n;
    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        if (p == last) repeat = true;
        else
        {
            if (count < 3) primes[count] = p;
            ++count;
            last = p;
        }
        tmp /= p;
    }

    if (repeat || count != 3) return false;

    uint64_t l1 = digit_count(primes[0]);
    uint64_t l2 = digit_count(primes[1]);
    uint64_t l3 = digit_count(primes[2]);
    return (l1 != l2 && l1 != l3 && l2 != l3);
}

/* Ensures that the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    cache = new_arr;
    cache_cap = new_cap;
}

/* Scans integers from 'start' up to and including 'limit',
 * adding qualifying ones to the cache. */
static void scan_range(uint32_t start, uint32_t limit)
{
    for (uint32_t x = start; x <= limit; ++x)
    {
        if (is_candidate(x))
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)x;
        }
    }
    if (limit > max_checked) max_checked = limit;
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence begins at index 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        uint32_t next_limit;
        if (max_checked == 0)                 /* first trial */
            next_limit = 10000000U;
        else
        {
            next_limit = max_checked * 2;
            if (next_limit < max_checked + 1000) next_limit = max_checked + 1000;
        }

        build_spf(next_limit);
        scan_range((max_checked == 0) ? 2U : max_checked + 1, next_limit);
    }

    return cache[n - 1];
}

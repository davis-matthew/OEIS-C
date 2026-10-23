/*
 * A125499  Deficient even numbers.
 * https://oeis.org/A125499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Deficient even numbers – A125499
   a(n) is the n‑th even positive integer e such that σ(e) < 2e
   (σ(e) is the sum of all divisors of e).
   The routine searches even numbers and uses a static cache so that
   later requests for larger indices are fast.  It is valid C11 and C++17. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Compute σ(e) for a 64‑bit unsigned integer e.  The result fits in
    idx_t (which is uint64_t).  */
static idx_t sigma_uint64(uint64_t e)
{
    uint64_t n = e;
    idx_t sigma = 1;

    /* Handle factor 2 separately. */
    if ((n & 1) == 0) {
        int exp = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++exp;
        }
        /* (2^(exp+1)-1)/(2-1) = 2^(exp+1)-1 */
        sigma *= (((idx_t)1) << (exp + 1)) - 1;
    }

    /* Handle odd prime factors. */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            /* Compute (p^(exp+1)-1)/(p-1). */
            idx_t num = 1;
            for (int k = 0; k < exp + 1; ++k)
                num *= (idx_t)p;
            idx_t part = (num - 1) / ((idx_t)p - 1);
            sigma *= part;
        }
    }

    /* Remaining prime factor, if any. */
    if (n > 1) {
        /* (n^2-1)/(n-1) = n+1 */
        sigma *= ((idx_t)n + 1);
    }

    return sigma;
}

/* Return true iff e is even and σ(e) < 2e. */
static bool deficient_even(uint64_t e)
{
    if ((e & 1) != 0)
        return false;
    idx_t s = sigma_uint64(e);
    return s < ((idx_t)2 * (idx_t)e);
}

/* ------------------------------------------------------------------ */
/*  Cache used to store already found terms of the sequence.          */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of terms stored     */
static size_t cache_cap  = 0; /* allocated capacity         */
static uint64_t last_checked = 0; /* largest even e examined   */

/* Ensure that the cache can hold at least 'needed' entries. */
static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In a real implementation we might handle out‑of‑memory here. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑indexed).  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for index 0, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        uint64_t cand = (last_checked == 0) ? 2 : last_checked + 2;
        while (!deficient_even(cand))
            cand += 2; /* keep only even candidates */

        ensure_capacity(cache_size + 1);
        cache[cache_size++] = (seq_t)cand;
        last_checked = cand;
    }

    return cache[(size_t)n - 1];
}

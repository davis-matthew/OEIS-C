/*
 * A074202  Numbers k such that the number of 1's in the binary expansion of k
 *          divides 2^k-1.
 * https://oeis.org/A074202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 7241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074202: Numbers k such that popcount(k) divides 2^k-1.  
   The function returns the nth such integer (offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/popcount_idx.h"

/* Test whether k satisfies: popcount(k) | (2^k - 1). */
static inline bool ok_k(idx_t k)
{
    unsigned pc = popcount_idx(k);
    if (pc == 1)         /* 1 divides everything */
        return true;

    unsigned long long mod = pc;
    unsigned long long r = 1 % mod;
    unsigned long long b = 2 % mod;

    idx_t e = k;
    while (e) {
        if (e & 1) r = (r * b) % mod;
        b = (b * b) % mod;
        e >>= 1;
    }
    return r == 1 % mod;
}

/* Cache of found terms. */
static seq_t        *cache      = NULL;
static size_t       cache_len   = 0;
static size_t       cache_cap   = 0;
static idx_t        next_k      = 1;

/* Ensure the cache has enough capacity. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
        while (new_cap < need) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Add a term to the cache. */
static void add_term(idx_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = (seq_t)val;
}

/* Generate terms until we have at least n of them. */
static void generate_up_to(size_t n)
{
    while (cache_len < n) {
        if (ok_k(next_k))
            add_term(next_k);
        ++next_k;
    }
}

/* External linkage: returns the nth term (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, harmless */
    generate_up_to((size_t)n);
    return cache[n - 1];
}

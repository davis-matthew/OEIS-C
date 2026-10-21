/*
 * A224486  Numbers k such that 2*k+1 divides 2^k+1.
 * https://oeis.org/A224486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3636, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A224486: numbers k such that (2*k+1) divides (2^k+1), computed via brute‑force search with caching. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Cache of found terms (k values). */
static seq_t *cache = NULL;          /* array holding the sequence values */
static size_t cache_cap = 0;         /* current capacity of the cache */
static size_t cache_len = 0;         /* number of terms stored so far */
static idx_t next_test_k = 1;        /* next k to test */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Find the n‑th term of A224486. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        idx_t k = next_test_k++;
        seq_t m = ((seq_t)2 * (seq_t)k) + (seq_t)1;   /* 2k+1   */
        seq_t pow_mod = mod_pow((seq_t)2, (uint64_t)k, m);
        /* Check 2^k ≡ -1 (mod m), i.e. pow_mod == m-1. */
        if (pow_mod == m - (seq_t)1) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}

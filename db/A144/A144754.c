/*
 * A144754  Integers that have a prime number of 0's in their binary
 *          expansion.
 * https://oeis.org/A144754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th positive integer whose binary representation
 *            contains a prime number of 0's
 * Range:     u8 n <= 155, u16 n <= 9395, u32 n <= 9395, u64 n <= 9395,
 *            u128 n <= 9395, big any n
 * Helpers:   count_binary_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_binary_zeros.h"

/* ---------- helper: prime test for zero counts ---------- */
static bool is_prime_uint64(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & (idx_t)1) == (idx_t)0) return false;   /* even */
    for (idx_t d = (idx_t)3; d <= n / d; d += (idx_t)2) {
        if (n % d == (idx_t)0) return false;
    }
    return true;
}

/* ---------- static cache to store sequence values ---------- */
static seq_t *cache = NULL;          /* array of found terms      */
static size_t   cache_cap = 0;       /* capacity of the array    */
static size_t   cache_size = 0;      /* number of stored terms    */
static seq_t   next_candidate = 0;   /* last number examined     */

/* ---------- append new term to the cache, growing if needed ---------- */
static void append_term(seq_t val)
{
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
    } else if (cache_size == cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, safe return */

    /* generate sequence terms until we have at least n terms */
    while (cache_size < n) {
        next_candidate++;                            /* examine next integer */
        idx_t zeros = count_binary_zeros(next_candidate);
        if (is_prime_uint64(zeros)) {
            append_term(next_candidate);
        }
    }
    return cache[n - 1];          /* zero‑based index */
}

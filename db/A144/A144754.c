/*
 * A144754  Integers that have a prime number of 0's in their binary
 *          expansion.
 * https://oeis.org/A144754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th positive integer whose binary representation
 *            contains a prime number of 0's
 * Range:     u8 n <= 155, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 83 terms through n = 9395 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ---------- helper: count zeros in the binary expansion of x ----------
   Works for any seq_t that supports bitwise &, >> and comparison with 0.  */
static idx_t count_binary_zeros(seq_t x)
{
    seq_t y = x;
    idx_t bits = 0;
    idx_t ones = 0;
    while (y != (seq_t)0) {
        bits++;
        if ((y & (seq_t)1) != (seq_t)0) {
            ones++;
        }
        y >>= 1;
    }
    return bits - ones;
}

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
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    } else if (cache_size == cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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

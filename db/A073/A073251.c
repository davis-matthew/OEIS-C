/*
 * A073251  Numbers k such that k, k+1 and k+2 are nonprime and squarefree.
 * https://oeis.org/A073251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 4952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A073251: numbers k such that k, k+1 and k+2 are nonprime and squarefree.
    Method: brute‑force search with caching of previously found terms.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* Cached sequence terms */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;   /* number of terms in cache      */
static size_t cache_cap      = 0;   /* allocated capacity (in terms) */

/* Smallest k not yet tested; starts above the smallest possible term (k ≥ 4). */
static uint64_t next_k  = 4;

/* Append a new term to the cache, reallocating if necessary. */
static void append_term(uint64_t k)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)k;
}

/* Test whether k, k+1, k+2 are all composite and square‑free. */
static bool candidate_valid(uint64_t k)
{
    uint64_t k1 = k + 1;
    uint64_t k2 = k + 2;

    /* Prime check: uses idx_t (uint64_t) version */
    if (is_prime(k) || is_prime(k1) || is_prime(k2))
        return false;

    /* Square‑free check: use seq_t version */
    if (!is_squarefree((seq_t)k) ||
        !is_squarefree((seq_t)k1) ||
        !is_squarefree((seq_t)k2))
        return false;

    return true;
}

/* Return the n-th term of the sequence (n ≥ 1).  */
seq_t a(idx_t n)
{
    /* Build the cache up to the requested index. */
    while (cache_len < (size_t)n) {
        /* Find the next valid k. */
        do { ++next_k; } while (!candidate_valid(next_k));
        append_term(next_k);
    }
    return cache[n - 1];
}

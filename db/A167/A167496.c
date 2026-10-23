/*
 * A167496  Primes p such that (p reversed)-10 is also a prime.
 * https://oeis.org/A167496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that rev_decimal(p)-10 is prime
 *            (rev_decimal(p) is the decimal digit reversal of p)
 * Range:     u8 n <= 4, u16 n <= 140, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/rev.h"          /* idx_t rev(idx_t) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */

static seq_t *cache = NULL;     /* Cached sequence values */
static size_t cache_len = 0;    /* Number of cached values */
static size_t cache_cap = 0;    /* Allocated capacity */
static idx_t  next_candidate = 2; /* Next integer to test */

/* Append a new term to the cache, resizing if needed. */
static void _append_term(seq_t term)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* In production code handle error */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = term;
}

/* Return the n-th term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Defined safely although the offset is 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t cand = next_candidate++;
        if (!is_prime_big((seq_t)cand)) continue;  /* Candidate must itself be prime */

        idx_t rev_val = rev(cand);
        if (rev_val < 10) continue;                /* rev(p)-10 would be negative */

        seq_t rev_minus = (seq_t)(rev_val - 10);
        if (!is_prime_big(rev_minus)) continue;    /* reversed−10 must also be prime */

        _append_term((seq_t)cand);
    }

    return cache[n - 1];
}

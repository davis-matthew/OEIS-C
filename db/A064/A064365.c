/*
 * A064365  a(0) = 0; thereafter a(n) = a(n-1)-prime(n) if positive and new,
 *          otherwise a(n) = a(n-1)+prime(n), where prime(n) is the n-th
 *          prime.
 * https://oeis.org/A064365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, look, hear
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1995, u32 n <= 9557, u64 n <= 9557,
 *            u128 n <= 9557, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  a(0) = 0.
 * For n ≥ 1,
 *   a(n) = a(n‑1) − prime(n)   if the result is positive and new;
 *            else a(n) = a(n‑1) + prime(n).
 * prime(n) is the n‑th prime (prime(1)=2).  The sequence may have repeated
 * values only by addition.
 * The implementation memoises computed terms so calls may arrive in
 * arbitrary order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k) returns the k‑th prime */

/* static cache of computed a(k) values */
static seq_t *seq_cache = NULL;   /* pointer to array of a(k) values          */
static size_t  cache_len  = 0;    /* number of values stored (>=1 after init) */
static size_t  cache_cap  = 0;    /* allocated capacity of seq_cache          */

/* return true iff val appears among seq_cache[0 .. up_to-1] */
static bool seq_seen(seq_t val, size_t up_to)
{
    for (size_t j = 0; j < up_to; ++j)
        if (seq_cache[j] == val)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (seq_cache == NULL) {
        cache_cap = 1;
        cache_len = 1;                     /* we store a(0) only for now    */
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) abort();           /* allocation failed              */
        seq_cache[0] = 0;                  /* a(0) = 0                       */
    }

    /* if already computed up to n, just return it */
    if (n < cache_len)
        return seq_cache[(size_t)n];

    /* grow cache up to n inclusive */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        size_t idx = (size_t)i;            /* index into seq_cache          */

        /* ensure capacity */
        if (idx >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 1;  /* safety for first growth     */
            seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();             /* allocation failed            */
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        /* compute a(i) */
        seq_t p     = (seq_t)get_prime(i);     /* i starts at 1; get_prime(1)=2  */
        seq_t prev  = seq_cache[idx - 1];
        seq_t value;

        if (prev > p) {                       /* subtraction would be positive */
            seq_t cand = prev - p;
            if (!seq_seen(cand, idx))
                value = cand;
            else
                value = prev + p;
        } else {                               /* subtraction would be ≤0       */
            value = prev + p;
        }

        seq_cache[idx] = value;
    }

    cache_len = n + 1;
    return seq_cache[(size_t)n];
}

/*
 * A020491  Numbers k such that sigma_0(k) divides phi(k).
 * https://oeis.org/A020491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   divisor_and_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020491: numbers k such that σ0(k) divides φ(k).
   The implementation builds the sequence lazily by testing each integer
   in increasing order until the required nth term is found.  No
   pre‑computed tables or large sieves are used; simple trial division
   provides all needed facts.  This guarantees correctness for any
   seq_t implementation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_and_phi.h"

/* Cache of previously found qualifying numbers (1‑indexed). */
static seq_t *cache  = NULL;        /* array of found values          */
static size_t cache_len = 0;        /* current number of stored terms */
static uint64_t last_k = 0;         /* last integer examined          */

/* Append a new term to the cache, growing it if necessary. */
static void cache_append(uint64_t val)
{
    if (cache_len == 0) {
        size_t cap = 64;                /* initial capacity              */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache_len = 0;
    }
    /* Grow cache when needed. */
    if (cache_len >= cache_len) {
        size_t new_cap = cache_len ? cache_len * 2 : 64;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
    }
    cache[cache_len++] = (seq_t)val;    /* cast to the build's seq_t type   */
}

/* Public interface; returns the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* undefined for n<1 */
    while (cache_len < (size_t)n) {                 /* generate until enough terms */
        ++last_k;
        uint64_t d, phi;
        divisor_and_phi(last_k, &d, &phi);
        if (phi % d == 0) {
            cache_append(last_k);
        }
    }
    return cache[n - 1];
}

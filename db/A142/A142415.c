/*
 * A142415  Primes congruent to 2 mod 49.
 * https://oeis.org/A142415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142415: primes congruent to 2 modulo 49. */
/* The function computes the n‑th such prime by incremental search
 * and memoises results.  No formula is applied. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* prototype: bool is_prime(idx_t x); */

/* Internal helper to grow a dynamic array. */
static void grow_cache(seq_t **p, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : *cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* memory error: terminate */
    *p = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache of computed primes. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;      /* number of stored terms */
    static idx_t next_k = 0;    /* k such that candidate = 49*k + 2 */

    if (n == 0) return 0; /* offset is 1, behavior for n<1 is irrelevant */

    /* Return cached value if already computed. */
    if (n <= len) return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        idx_t cand = next_k * 49 + 2;       /* candidate = 49*k + 2 */
        if (is_prime(cand)) {
            grow_cache(&cache, &cap, len + 1);
            cache[len++] = (seq_t)cand;
        }
        ++next_k;
    }
    return cache[n - 1];
}

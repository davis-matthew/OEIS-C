/*
 * A172458  Nonprimes n such that exactly one of 2n-1 and 2n+1 is prime.
 * https://oeis.org/A172458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sequence of non‑prime n for which exactly one of 2n‑1 and 2n+1 is prime.
   The function keeps a static cache of the found terms and expands it on demand. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"        /* provides bool is_prime(idx_t) */

static seq_t *cache = NULL;      /* cache of sequence terms (0‑based) */
static size_t cache_size = 0;    /* number of terms currently cached */
static size_t cache_cap  = 0;    /* allocated capacity of cache */
static idx_t next_candidate = 1; /* smallest integer not yet examined */

/* Ensure that the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* insufficient memory */
    cache = tmp;
    cache_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0; return 0 as sentinel */

    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];   /* already computed */

    /* Generate new terms until the cache contains at least n entries. */
    while (cache_size < n) {
        /* Skip prime candidates; we require a non‑prime n. */
        if (!is_prime(next_candidate)) {
            idx_t two = next_candidate << 1;   /* 2 * next_candidate */
            bool p1 = is_prime(two - 1);
            bool p2 = is_prime(two + 1);
            if (p1 ^ p2) {                     /* exactly one is prime */
                ensure_cache(cache_size + 1);
                cache[cache_size] = (seq_t)next_candidate;
                ++cache_size;
            }
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}

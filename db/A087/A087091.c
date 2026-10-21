/*
 * A087091  Numbers k such that p = k^2 + k + 41 and p - 2 are twin primes.
 * https://oeis.org/A087091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 31, u32 n <= 2619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that k^2 + k + 41 and k^2 + k + 39 are both prime.
   Return the n‑th such k (1‑indexed).  This is the definition of
   OEIS A087091.  The sequence consists of the values of k. */

/* The build supplies the integer types seq_t and idx_t, along with
   the helper isPrimeGeneric(seq_t).  No other special facilities are
   required.  All arithmetic is performed on seq_t, which may be
   arbitrarily large. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;      /* stored terms */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t k_next = 0;         /* next k to test */

/* Expand the cache if it does not have the required capacity.  This
   routine is written in a way that is valid both in C and C++ (the
   cast from void* to seq_t* is required under C++). */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1024;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th element of the sequence (1‑based).  If n == 0, the
   sequence is undefined; the function simply returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defined only for n >= 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)k_next;                     /* current k */
        seq_t p = k * k + k + (seq_t)41;              /* p = k^2 + k + 41 */
        seq_t p_minus = p - (seq_t)2;                /* p-2 */

        if (isPrimeGeneric(p) && isPrimeGeneric(p_minus)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = k;
        }
        ++k_next;
    }
    return cache[n - 1];
}

/*
 * A139533  Numbers k such that numbers 24*k + 11 and 24*k + 13 are twin
 *          primes.
 * https://oeis.org/A139533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that 24*k+11 and 24*k+13 are both
 *            prime
 * Range:     u8 n <= 5, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cache of k values */
static seq_t *k_cache      = NULL;   /* array of k values */
static size_t k_cache_cap  = 0;      /* allocated capacity */
static size_t k_cache_sz   = 0;      /* number of stored values */

/* Next k to evaluate (0‑based counter) */
static idx_t next_k_to_test = 0;

/* Expand the cache to hold at least ‘needed’ elements. */
static void grow_k_cache(size_t needed)
{
    if (needed <= k_cache_cap)
        return;
    size_t new_cap = k_cache_cap ? k_cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* memory allocation failed */
    k_cache = tmp;
    k_cache_cap = new_cap;
}

/* Retrieve the n-th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined for n<1 */

    /* Generate k values until we have at least n of them. */
    while (k_cache_sz < n) {
        idx_t k = next_k_to_test++;
        seq_t p1 = (seq_t)k * 24 + 11;  /* first twin prime candidate */
        seq_t p2 = (seq_t)k * 24 + 13;  /* second twin prime candidate */

        /* Test the twin‑prime condition. */
        if (is_prime((idx_t)p1) && is_prime((idx_t)p2)) {
            grow_k_cache(k_cache_sz + 1);
            k_cache[k_cache_sz++] = (seq_t)k;
        }
    }

    return k_cache[n - 1];
}

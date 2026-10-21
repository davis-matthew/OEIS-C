/*
 * A097143  a(n) is the least prime greater than a(n-1) such that n is a
 *          quadratic residue mod p.
 * https://oeis.org/A097143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least prime > a(n‑1) such that n is a quadratic
 *            residue modulo p. The sequence is computed lazily and cache‐wise
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"

/* static cache of computed terms: element 0 is a(1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */

/* return the n‑th term of A097143 (n >= 1).  Undefined for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* behaviour below offset is irrelevant */

    /* Allocate or grow the cache to accommodate n terms. */
    if (!cache) {
        cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
    }
    if ((size_t)n > cache_len) {
        size_t old_len = cache_len;
        cache_len = (size_t)n;                /* new desired length      */
        seq_t *tmp = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;

        /* The very first term is fixed. */
        if (old_len == 0) {
            cache[0] = (seq_t)2;
            old_len = 1;
        }

        /* Compute any missing terms from old_len up to n-1. */
        for (idx_t k = old_len; k < (idx_t)cache_len; ++k) {
            seq_t prev = cache[k - 1];          /* a(k)  where k = n-1 for this loop */
            uint64_t start = (uint64_t)prev + 1;
            uint64_t p = next_prime(start);     /* first prime > prev */

            /* The index of the term being computed is k+1. */
            uint64_t nn = (uint64_t)(k + 1);

            /* Find the first prime > prev with Jacobi symbol (nn|p) == 1. */
            while (jacobi_symbol(nn, p) != 1) {
                p = next_prime(p + 1);
            }
            cache[k] = (seq_t)p;
        }
    }

    return cache[n - 1];   /* stored index 0 corresponds to a(1) */
}

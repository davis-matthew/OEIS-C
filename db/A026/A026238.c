/*
 * A026238  a(n) = j if n is the j-th prime, else a(n) = k if n is the k-th
 *          composite.
 * https://oeis.org/A026238
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for n ≥ 2, a(n) = j if n is the j‑th prime,
   otherwise a(n) = k if n is the k‑th composite. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cache the already computed sequence values and the cumulative counts. */
static seq_t *seq_cache   = NULL;   /* a(n) values, indexed by n */
static idx_t  cache_size  = 0;      /* allocated array size */
static idx_t  last_computed = 1;    /* highest n already processed */
static idx_t  prime_cnt   = 0;      /* number of primes ≤ last_computed */
static idx_t  comp_cnt    = 0;      /* number of composites ≤ last_computed */

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* region before the offset – arbitrary */

    /* Ensure the cache array can hold indices up to n. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – give up */
        seq_cache = tmp;
        cache_size = new_size;
    }

    /* Compute values only for indices that have not yet been processed. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (is_prime(i)) {
            ++prime_cnt;
            seq_cache[i] = (seq_t)prime_cnt;
        } else {
            ++comp_cnt;
            seq_cache[i] = (seq_t)comp_cnt;
        }
    }
    last_computed = n;

    return seq_cache[n];
}

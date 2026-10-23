/*
 * A112141  Product of the first n semiprimes.
 * https://oeis.org/A112141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of the first n semiprimes (A001358)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   is_semiprime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_semiprime_uint64.h"

#ifdef __cplusplus
#include <new>
#include <algorithm>
#else
#include <stdlib.h>
#include <string.h>
#endif

/* Computes a(n) = Π_{i=1..n} A001358(i), where A001358(i)
 * is the i-th semiprime (a number with exactly two prime factors,
 * counted with multiplicity). The function caches results and
 * works correctly for any order of calls. */

/* static cache of computed products */
static seq_t *cache = NULL;   /* 1‑based indexing: cache[1] = a(1) */
static idx_t cache_cap = 0;   /* allocated capacity (number of elements) */
static idx_t cache_last = 0;  /* highest n that has been computed */

static idx_t semiprime_candidate = 4;                 /* next integer to test */
static seq_t current_product = (seq_t)1;              /* product of all found semiprimes */

/* ensure capacity for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    if (new_cap <= n) new_cap = n + 8; /* a little extra space */

#if defined(__cplusplus)
    seq_t *newp = new seq_t[new_cap];
    if (cache) {
        std::copy(cache, cache + cache_cap, newp);
        delete[] cache;
    }
    cache = newp;
#else
    seq_t *newp = malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(newp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
#endif
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 */

    if (n <= cache_last) return cache[n];

    ensure_capacity(n);

    /* extend computation up to n */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        /* find next semiprime */
        while (!is_semiprime_uint64(semiprime_candidate)) ++semiprime_candidate;

        /* multiply into running product */
        current_product = current_product * (seq_t)semiprime_candidate;

        /* store result */
        cache[k] = current_product;

        /* advance to next candidate for next loop iteration */
        ++semiprime_candidate;
    }

    cache_last = n;
    return cache[n];
}

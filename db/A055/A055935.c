/*
 * A055935  a(0)=1; a(n) = Sum_{j<n, gcd(n,a(j)) = 1} a(j).
 * https://oeis.org/A055935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 72, u64 n <= 150,
 *            u128 n <= 302, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055935
   a(0) = 1
   a(n) = Σ{ j<n | gcd(n , a(j)) = 1 } a(j)   for n > 0
   (using the definition from OEIS) */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* memoised values of a(n)   */
static idx_t max_computed = (idx_t)-1;  /* highest n computed so far */
static size_t capacity = 0;                   /* allocated cache size */

/* Ensure that cache is large enough to hold values up to n. */
static void
ensure_cache(idx_t n)
{
    if (capacity > (size_t)n)
        return;
    size_t new_cap = capacity ? capacity << 1 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    capacity = new_cap;
}

/* Compute sequence value using memoisation. */
seq_t a(idx_t n)
{
    /* initialise base cases on first call */
    if (!cache) {
        ensure_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n <= max_computed)
        return cache[n];

    /* Compute new entries up to n. */
    ensure_cache(n);
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k > 1 && ((k & 1) == 0)) {
            /* Even indices (except 0) are 2. */
            cache[k] = (seq_t)2;
        } else {
            /* Odd n: sum over all j < k with gcd(k, a(j)) == 1. */
            seq_t sum = 0;
            seq_t kseq = (seq_t)k;
            for (idx_t j = 0; j < k; ++j) {
                if (gcd(kseq, cache[j]) == (seq_t)1)
                    sum += cache[j];
            }
            cache[k] = sum;
        }
    }
    max_computed = n;
    return cache[n];
}

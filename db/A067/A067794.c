/*
 * A067794  Nonprime numbers k such that phi(k) > 3*k/4.
 * https://oeis.org/A067794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime k such that φ(k) > 3*k/4
 * Range:     u8 n <= 28, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by incremental search with caching.      */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/calc_phi.h"

/* --- cached sequence ------------------------------------- */
static seq_t *cache      = NULL;   /* stored terms                */
static size_t cache_cap  = 0;      /* capacity of cache array     */
static size_t cache_len  = 0;      /* number of terms stored      */

/* --- next integer to test ------------------------------- */
static idx_t next_candidate = 1;

/* --- ensure at least needed entries in the cache --------- */
static void
ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (needed > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* graceful failure         */
    cache = tmp;
    cache_cap = new_cap;
}

/* --- main sequence function ---------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset     */
    if (n <= cache_len) return cache[n - 1];

    while (cache_len < n) {
        idx_t k = next_candidate;          /* integer candidate to test */

        if (!is_prime(k)) {                /* consider only non‑primes   */
            seq_t phi_k = calc_phi((seq_t)k);
            /* use 64‑bit arithmetic to avoid overflow on small seq_t   */
            if ((uint64_t)phi_k * 4 > 3 * (uint64_t)k) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
            }
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

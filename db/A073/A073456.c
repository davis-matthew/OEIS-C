/*
 * A073456  Numbers n such that phi(n) = pi(n)+1.
 * https://oeis.org/A073456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   calc_phi, is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073456: Numbers n such that φ(n) = π(n)+1.
   Implementation: sequential search with memoised cache.  */
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/is_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cached sequence values            */
static size_t cache_cap = 0;     /* allocated capacity in cache       */
static size_t cache_size = 0;    /* number of stored terms            */
static idx_t  primes_cnt = 0;    /* count of primes discovered so far */
static idx_t  last_checked = 0;  /* highest n already examined        */

/* Ensure the internal cache can hold at least 'need' terms. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Extend the cached sequence until it contains at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    if (cache_size >= (size_t)need)
        return;

    ensure_cache_capacity(need);

    while (cache_size < (size_t)need)
    {
        last_checked += 1;

        /* Update prime count. */
        if (is_prime(last_checked))
            primes_cnt += 1;

        seq_t pi_n   = (seq_t)primes_cnt;          /* π(n) */
        seq_t phi_n  = calc_phi((seq_t)last_checked); /* φ(n) */

        if (phi_n == pi_n + (seq_t)1)
        {
            cache[cache_size++] = (seq_t)last_checked;
        }
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset starts at 1; return 0 for 0-index nonsense */
        return (seq_t)0;

    ensure_terms(n);
    return cache[n - 1];
}

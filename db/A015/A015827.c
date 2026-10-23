/*
 * A015827  Numbers k such that phi(k + 9) | sigma(k).
 * https://oeis.org/A015827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 653, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015827: Numbers k such that phi(k + 9) | sigma(k) */

/* The function performs a linear search for the requested index.
 * It iteratively tests each k, computing sigma(k) with the
 * provided sigma_of() and phi(k+9) with a small factorisation
 * routine that uses factor_u64().  The results are cached so
 * subsequent calls (in any order) are fast.  The algorithm
 * runs in O(n) time for the n-th element and stays well below
 * the 2‑second limit for n up to about 10000. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;      /* array of found terms */
static size_t cache_len  = 0;    /* number of stored terms */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  next_k     = 1;    /* next k to test */

/* compute phi(m) using a simple prime‑factorisation */
static seq_t compute_phi(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    seq_t phi = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = pr[i];
        seq_t pow = 1;
        for (size_t j = 1; j < ex[i]; ++j) pow *= p;   /* p^(e-1)   */
        seq_t term = pow * (p - 1);
        phi *= term;
    }
    return phi;
}

/* grow the cache array when necessary */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_ptr = NULL;
    if (new_cap == 0)
        new_ptr = NULL;
    else if (cache == NULL)
        new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* find and store terms up to at least the requested index */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        ++next_k;
        seq_t sigma = sigma_of((idx_t)next_k);      /* σ(k)   */
        seq_t phi   = compute_phi((uint64_t)(next_k + 9)); /* φ(k+9) */
        if (sigma % phi == 0) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* zero index is out of range for this sequence  */
    ensure_terms(n);
    return cache[n - 1];
}

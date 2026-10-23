/*
 * A166718  Numbers with at most 4 prime factors (counted with multiplicity).
 * https://oeis.org/A166718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate the numbers with at most 4 prime factors
 * (counted with multiplicity) by incrementally sieving for the
 * smallest prime factors and counting factors.  The function a(n)
 * returns the n‑th such number, 1‑indexed. */
#include <stdint.h>
#include <stdlib.h>

/* smallest‑prime‑factor table ------------------------------------------------ */
static idx_t *spf = NULL;      /* spf[i] = smallest prime factor of i   */
static idx_t  spf_cap = 0;     /* up to index spf_cap-1 are valid */

/* ensure that spf contains entries for all numbers <= limit */
static void ensure_spf(idx_t limit)
{
    if (limit < spf_cap) return;      /* already enough */

    if (spf) free(spf);               /* recompute from scratch */
    idx_t need = limit + 1;
    spf = (idx_t*)calloc(need, sizeof(idx_t));
    if (!spf) exit(1);

    for (idx_t i = 0; i < need; ++i) spf[i] = i;
    if (need > 0) spf[0] = 0;
    if (need > 1) spf[1] = 1;

    for (idx_t i = 2; i <= limit / i; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_cap = need;
}

/* count of prime factors (with multiplicity) of n, using spf table */
static idx_t smallOmega(idx_t n)
{
    if (n <= 1) return 0;
    idx_t cnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        cnt++;
        n /= p;
    }
    return cnt;
}

/* cache of sequence terms ----------------------------------------------------
 * cache[k] holds a(k+1).  The cache is filled lazily. */
static seq_t *cache = NULL;
static size_t   cache_cap   = 0;
static size_t   cache_size  = 0;
static idx_t    next_candidate = 1;   /* integer to test next */

/* extend cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        idx_t cand = next_candidate;
        ensure_spf(cand);
        if (smallOmega(cand) <= 4) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)cand;   /* construct seq_t from cand */
        }
        ++next_candidate;
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but avoid UB */
    ensure_cache(n);
    return cache[n - 1];
}

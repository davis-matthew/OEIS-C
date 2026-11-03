/*
 * A133759  Numbers that are the sum of a nonzero pentagonal number and a
 *          nonzero square in at least one way.
 * https://oeis.org/A133759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n‑th integer that can be written as P(i)+j²,
   where P(i)=i(3i‑1)/2 is a pentagonal number and i>0, j>0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Binary search for target in sorted array pent[0..sz-1]. */
static inline bool contains_pentagonal(const seq_t *pent, size_t sz, seq_t target)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t v = pent[mid];
        if (v < target)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < sz) && (pent[lo] == target);
}

/* Pentagonal number for k>0. */
static inline seq_t compute_pentagonal(idx_t k)
{
    return (seq_t)k * ((seq_t)3 * k - 1) / 2;
}

/* Ensure that all pentagonal numbers ≤ limit are stored in *pent.
   The pointer *pent, its size *sz and capacity *cap are updated if
   a reallocation occurs.  next_k holds the next index to generate. */
static void ensure_pentagonals_up_to(seq_t **pent, size_t *sz, size_t *cap,
                                     idx_t *next_k, seq_t limit)
{
    for (;;) {
        seq_t p = compute_pentagonal(*next_k);
        if (p > limit)
            break;

        if (*sz == *cap) {
            size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
            seq_t *tmp = mem_resize(seq_t, *pent, new_cap);
            if (!tmp)
                abort();
            *pent = tmp;
            *cap = new_cap;
        }
        (*pent)[(*sz)++] = p;
        (*next_k)++;
    }
}

/* Check whether m can be expressed as P(i)+j² with i,j>0.
   The array *pent must contain all pentagonal numbers ≤ m‑1. */
static bool can_represent(seq_t m,
                          seq_t **pent, size_t *sz, size_t *cap, idx_t *next_k)
{
    ensure_pentagonals_up_to(pent, sz, cap, next_k, m - 1);

    for (idx_t j = 1; ; ++j) {
        seq_t j2 = (seq_t)j * j;
        if (j2 >= m)
            break;
        seq_t r = m - j2;
        if (contains_pentagonal(*pent, *sz, r))
            return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;

    /* Pentagonal numbers storage */
    static seq_t *pent = NULL;
    static size_t pent_sz = 0, pent_cap = 0;
    static idx_t next_pent_k = 1;

    while (cache_sz < n) {
        seq_t m;
        if (cache_sz == 0)
            m = 2;                    /* first term */
        else
            m = cache[cache_sz - 1] + 1;

        while (!can_represent(m,
                              &pent, &pent_sz, &pent_cap, &next_pent_k))
            ++m;

        if (cache_sz == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = m;
    }
    return cache[n - 1];
}

/*
 * A130501  a(1)=1, a(n) = number of earlier terms of the sequence that divide
 *          (the sum of positive integers which are <=n and missing from first
 *          (n-1) terms of sequence).
 * https://oeis.org/A130501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130501
   a(1)=1
   For n>1, a(n)= number of earlier terms that divide
   the sum of positive integers <=n that are missing from
   the first (n‑1) terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache of computed a(n) values (1‑indexed as stored) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* presence of values that have appeared in the sequence so far.
   presence[v] == true  iff  the integer v has appeared as a term
   among the first cache_len terms. */
static bool *present = NULL;
static size_t present_cap = 0;

/* grow cache to hold at least `need` elements */
static void ensure_cache_cap(size_t need) {
    if (need <= cache_cap) return;
    size_t newcap = cache_cap > 0 ? cache_cap * 2 : 16;
    if (newcap < need) newcap = need;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_cap = newcap;
}

/* grow presence array to hold indices up to `need` */
static void ensure_present_cap(size_t need) {
    if (need <= present_cap) return;
    size_t newcap = present_cap > 0 ? present_cap * 2 : 16;
    if (newcap < need) newcap = need;
    bool *tmp = (bool *)realloc(present, newcap * sizeof(bool));
    /* initialise new slots to false */
    for (size_t i = present_cap; i < newcap; ++i) tmp[i] = false;
    present = tmp;
    present_cap = newcap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range */
    if (cache_len == 0) {                 /* initialise first term */
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
        /* initialise presence array and mark 1 as present */
        ensure_present_cap(2);            /* need index 1 */
        present[1] = true;
    }
    if (cache_len < (size_t)n) {          /* need to extend sequence */
        for (idx_t cur = (idx_t)(cache_len + 1); cur <= n; ++cur) {
            /* compute sum of missing integers <= cur */
            seq_t S = (seq_t)0;
            /* ensure presence up to cur for checking missingness */
            ensure_present_cap(cur + 1);
            for (idx_t k = 1; k <= cur; ++k) {
                if (!present[k]) S += (seq_t)k;
            }
            /* count how many earlier terms divide S */
            seq_t cnt = 0;
            for (size_t j = 0; j < (size_t)(cur - 1); ++j) {
                seq_t term = cache[j];
                /* term is always >=1 for n>=2 (a(1)=1, and 1 always divides) */
                if (S % term == (seq_t)0) ++cnt;
            }
            /* store the computed value */
            ensure_cache_cap((size_t)cur);
            cache[cur - 1] = cnt;
            /* mark the new value as present for future sums */
            ensure_present_cap((size_t)cnt + 1);
            present[(size_t)cnt] = true;
        }
        cache_len = (size_t)n;
    }
    return cache[(size_t)(n - 1)];
}

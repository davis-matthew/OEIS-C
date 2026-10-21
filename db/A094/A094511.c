/*
 * A094511  Smallest number distinct from n, not occurring earlier and having
 *          in binary representation not more ones than n.
 * https://oeis.org/A094511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A094511.
   I.e. a(n) is the smallest positive integer m such that
     (1) m ≠ n,
     (2) m has not appeared as a(k) for any k < n,
     (3) popcount(m) ≤ popcount(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* popcount for a 64‑bit value */
static inline unsigned popcount_uint64(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
}
static inline unsigned popcount_idx(idx_t x)
{
    return popcount_uint64((uint64_t)x);
}
static inline unsigned popcount_seq(seq_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; c++; }
    return c;
}

/* ----- Disjoint‑set union of free integers ---------------------------- */
static size_t *dsu_parent = NULL;
static size_t dsu_size   = 0;

static size_t dsu_find(size_t x)
{
    if (x >= dsu_size) {
        size_t newsize = (x + 1) * 2U;
        size_t *tmp = (size_t *)realloc(dsu_parent, newsize * sizeof(size_t));
        if (!tmp) exit(1);            /* fatal if memory allocation fails */
        /* new entries start as unused (=0) */
        for (size_t i = dsu_size; i < newsize; ++i) tmp[i] = 0;
        dsu_parent = tmp;
        dsu_size   = newsize;
    }
    if (dsu_parent[x] == 0) return x;  /* free */
    size_t p = dsu_find(dsu_parent[x]);/* path compression */
    dsu_parent[x] = p;
    return p;
}

static void dsu_use(size_t x)
{
    /* Mark x as used by linking it to the next unused number */
    dsu_find(x + 1);                    /* ensure capacity */
    dsu_parent[x] = dsu_find(x + 1);
}

/* ----- Cache of previously computed terms ----------------------------- */
static seq_t *cache         = NULL;
static size_t cache_allocated = 0;    /* number of slots allocated   */
static size_t cache_last      = 0;    /* highest n for which a(n) known */

static void ensure_cache(idx_t n)
{
    if ((size_t)n > cache_allocated) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = cache_allocated; i < (size_t)n; ++i) tmp[i] = 0;
        cache = tmp;
        cache_allocated = (size_t)n;
    }
}

/* ----- Compute a single term (possibly finishing many preceding ones) */
static seq_t compute_seq(idx_t n)
{
    ensure_cache(n);
    if (cache_last >= (size_t)n) return cache[n - 1];   /* already known */

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        unsigned ones = popcount_idx(i);
        size_t cand = 1;
        for (;;) {
            cand = dsu_find(cand);                     /* next unused >= cand */
            if ((idx_t)cand == i) {                    /* cannot equal index */
                cand++;
                continue;
            }
            if (popcount_seq((seq_t)cand) > ones) {     /* too many ones */
                cand++;
                continue;
            }
            /* found suitable candidate */
            dsu_use(cand);
            cache[i - 1] = (seq_t)cand;
            break;
        }
    }
    cache_last = (size_t)n;
    return cache[n - 1];
}

/* ----- Public function required by the harness ------------------------ */
seq_t a(idx_t n)
{
    return compute_seq(n);
}

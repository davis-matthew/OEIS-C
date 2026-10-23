/*
 * A169747  a(1)=1; thereafter, a(2n)=a(n), a(2n+1) = a(2n-1)-a(n) if that
 *          number is positive and not already in the sequence, otherwise
 *          a(2n+1) = a(2n-1)+a(n).
 * https://oeis.org/A169747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(2n)=a(n); a(2n+1)=a(2n-1)-a(n) if >0 and not already
 *            in the sequence, else a(2n-1)+a(n)
 * Range:     u8 n <= 110, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/* Lightweight open‑addressing hash set for seq_t values (sentinel 0).       */
/* ------------------------------------------------------------------------- */
static seq_t *hash_tbl   = NULL;   /* table of stored values */
static size_t hash_cap   = 0;      /* power of two capacity (>0)       */
static size_t hash_cnt   = 0;      /* number of elements inserted     */

/* Simple hash: fold 128‑bit values into size_t.                           */
static inline size_t hash_seq_t(seq_t v)
{
#if defined(__SIZEOF_INT128__)
    uint64_t lo = (uint64_t)v;
    uint64_t hi = (uint64_t)((unsigned __int128)v >> 64);
    return (size_t)(lo ^ hi);
#else
    return (size_t)v;
#endif
}

/* Ensure the hash table has capacity for at least 'need' elements.       */
static void hash_ensure_cap(size_t need)
{
    if (hash_cap != 0 && hash_cap >= need) return;

    size_t new_cap = hash_cap ? hash_cap : 8;
    while (new_cap < need) new_cap <<= 1;          /* power of two */

    /* Reallocate and rehash existing entries. */
    seq_t *new_tbl = (seq_t*)calloc(new_cap, sizeof(seq_t));
    if (!new_tbl) abort();

    /* Rehash old elements. */
    for (size_t i = 0; i < hash_cap; ++i) {
        seq_t v = hash_tbl[i];
        if (v == 0) continue;
        size_t h = hash_seq_t(v) & (new_cap-1);
        while (new_tbl[h] != 0) h = (h+1) & (new_cap-1);
        new_tbl[h] = v;
    }
    free(hash_tbl);
    hash_tbl = new_tbl;
    hash_cap = new_cap;
}

/* Test membership. */
static bool hash_contains(seq_t v)
{
    if (v == 0) return false;
    size_t h = hash_seq_t(v) & (hash_cap-1);
    while (hash_tbl[h] != 0) {
        if (hash_tbl[h] == v) return true;
        h = (h+1) & (hash_cap-1);
    }
    return false;
}

/* Insert without duplication. */
static void hash_insert(seq_t v)
{
    if (v == 0) return;
    hash_ensure_cap(hash_cnt + 1);
    size_t h = hash_seq_t(v) & (hash_cap-1);
    while (hash_tbl[h] != 0) {
        if (hash_tbl[h] == v) return;  /* already present */
        h = (h+1) & (hash_cap-1);
    }
    hash_tbl[h] = v;
    ++hash_cnt;
}

/* ------------------------------------------------------------------------- */
/* Cache for sequence terms.  cache[0] is unused for 1‑based indexing.      */
/* ------------------------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size (>=max_cached+1) */
static size_t max_cached = 0;

/* Ensure cache can hold indices up to n.                                 */
static void cache_ensure_cap(idx_t n)
{
    if (n <= max_cached) return;

    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 16) new_cap = 16;
    while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();

    /* Zero out newly added entries. */
    if (new_cap > cache_cap) {
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Main function as required.                                              */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n<1 */

    cache_ensure_cap(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if ((i & 1) == 0) {           /* even */
            val = cache[i/2];
        } else {                            /* odd: i = 2k+1, k>=1 */
            idx_t k = (i - 1) / 2;
            seq_t a2k1 = cache[2*k - 1];      /* a(2k-1) */
            seq_t ak    = cache[k];
            seq_t diff  = (a2k1 > ak) ? (a2k1 - ak) : (seq_t)0;
            if (diff > 0 && !hash_contains(diff))
                val = diff;
            else
                val = a2k1 + ak;
        }
        cache[i] = val;
        hash_insert(val);
    }

    max_cached = n;
    return cache[n];
}

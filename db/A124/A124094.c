/*
 * A124094  Table T(n,m) giving number of partitions of n such that all parts
 *          are coprime to m. Read along antidiagonals (increasing n,
 *          decreasing m).
 * https://oeis.org/A124094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 169, u16 n <= 1033, u32 n <= 8383, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A124094:  a(n) = T(n,m) read along antidiagonals.
    T(n,m) is the number of partitions of n into parts coprime to m.
    The function caches computed tables for each distinct m.  */
/* External integer types supplied by the build */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

typedef uint64_t idx_t;        /* index type (unsigned 64‑bit) */

/* Entry holding the DP table for a particular m */
struct entry {
    uint64_t m;          /* parameter m of T(n,m)            */
    size_t   dp_len;     /* number of dp entries computed    */
    seq_t   *dp;         /* dp[j] = T(j,m) for j < dp_len    */
};

/* Global cache of entries per m */
static struct entry **entries = NULL;  /* array of pointers to entries */
static size_t  entries_cnt = 0;        /* number of entries          */
static size_t  entries_cap = 0;        /* allocated size             */

/* Global cache for the antidiagonal sequence */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;         /* number of computed terms   */
static size_t a_cache_cap = 0;         /* allocated capacity         */

/* ------------------------------------------------------------------ */
/* Ensure capacity for the entries array */
static inline void ensure_entry_capacity(void)
{
    if (entries_cnt == entries_cap) {
        size_t new_cap = entries_cap ? entries_cap * 2 : 4;
        struct entry **tmp = (struct entry**)realloc(entries,
                            new_cap * sizeof(struct entry *));
        if (!tmp) abort();  /* fatal error */
        entries = tmp;
        entries_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Find an existing entry for m or create a new one */
static struct entry *find_or_create_entry(uint64_t m)
{
    for (size_t i = 0; i < entries_cnt; ++i)
        if (entries[i]->m == m) return entries[i];

    /* create new entry */
    ensure_entry_capacity();
    struct entry *e = (struct entry*)malloc(sizeof(struct entry));
    if (!e) abort();
    e->m = m;
    e->dp_len = 0;
    e->dp = NULL;
    entries[entries_cnt++] = e;
    return e;
}

/* ------------------------------------------------------------------ */
/* Integer square root by a simple loop (target range small) */
static inline idx_t isqrt64(idx_t n)
{
    idx_t r = 0;
    while ((r + 1ULL) * (r + 1ULL) <= n)
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/* Convert a linear index k into (n,m) coordinates on antidiagonals */
static inline void index_to_pair(idx_t k, idx_t *pn, idx_t *pm)
{
    /* Find S such that S*(S-1)/2 <= k < S*(S+1)/2 */
    idx_t sq = isqrt64(8 * k + 1);
    idx_t S = (sq - 1) / 2 + 1;

    idx_t prev = S * (S - 1) / 2;     /* elements before current diagonal */
    idx_t offset = k - prev;          /* position within diagonal (0‑based) */

    *pn = offset;
    *pm = S - offset;                /* m starts at 1 */
}

/* ------------------------------------------------------------------ */
/* Rebuild the dp array for a given entry up to target_n (inclusive) */
static void rebuild_dp(struct entry *e, size_t target_n)
{
    if (e->dp) {
        free(e->dp);
        e->dp = NULL;
        e->dp_len = 0;
    }

    seq_t *new_dp = (seq_t*)calloc((size_t)(target_n + 1), sizeof(seq_t));
    if (!new_dp) abort();

    new_dp[0] = (seq_t)1;               /* empty partition */

    seq_t m_seq = (seq_t)e->m;          /* convert m once */

    for (size_t coin = 1; coin <= target_n; ++coin) {
        if (gcd((seq_t)coin, m_seq) == (seq_t)1) {
            for (size_t j = coin; j <= target_n; ++j) {
                new_dp[j] = new_dp[j] + new_dp[j - coin];
            }
        }
    }

    e->dp = new_dp;
    e->dp_len = target_n + 1;
}

/* ------------------------------------------------------------------ */
/* Obtain T(n,m) via the entry cache */
static seq_t compute_T(idx_t n, uint64_t m)
{
    struct entry *e = find_or_create_entry(m);
    if (n + 1 > e->dp_len)
        rebuild_dp(e, (size_t)n);
    return e->dp[(size_t)n];
}

/* ------------------------------------------------------------------ */
/* Ensure a_cache can hold indices up to n inclusive */
static void ensure_a_cache_capacity(idx_t n)
{
    if (n >= a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* The function requested by the build */
seq_t a(idx_t n)
{
    if (n >= a_cache_len) {
        /* extend cache up to n, filling intermediate terms */
        ensure_a_cache_capacity(n);
        for (idx_t i = a_cache_len; i <= n; ++i) {
            idx_t tn, tm;
            index_to_pair(i, &tn, &tm);
            a_cache[i] = compute_T(tn, tm);
        }
        a_cache_len = n + 1;
    }
    return a_cache[n];
}

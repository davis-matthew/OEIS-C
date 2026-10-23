/*
 * A032995  If d,e are consecutive digits of n in base 7, then |d-e|>=5.
 * https://oeis.org/A032995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers whose base‑7 representation has no two
 * consecutive digits with absolute difference less than 5.
 * That is, for every adjacent pair of base‑7 digits d,e,
 * |d-e| >= 5.  The sequence lists such numbers in increasing order. */

#include <stdlib.h>    /* for malloc, realloc, free */

static seq_t *cache = NULL;     /* cached values */
static size_t cache_cap = 0;    /* current capacity of the cache */
static size_t cache_cnt = 0;    /* number of values stored in the cache */
static idx_t  produced_len = 0; /* length of numbers last produced */

/* Allowed successors for each base‑7 digit 0..6 */
static const int next_cnt[7] = {2,1,0,0,0,1,2};
static const int next_tbl[7][2] = {
    {5,6},   /* 0 -> 5, 6 */
    {6,0},   /* 1 -> 6, 0 */
    {0,0},   /* 2 -> none */
    {0,0},   /* 3 -> none */
    {0,0},   /* 4 -> none */
    {0,0},   /* 5 -> 0 */
    {0,1}    /* 6 -> 0, 1 */
};

/* Append a new value to the cache, growing it if needed. */
static inline void cache_push(seq_t v)
{
    if (cache_cnt == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failed */
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_cnt++] = v;
}

/* Recursive generation of all valid words of length L */
static void gen_len_rec(idx_t L, idx_t pos, seq_t val, int last)
{
    if (pos == L) {
        cache_push(val);
        return;
    }
    int cnt = next_cnt[last];
    for (int i = 0; i < cnt; ++i) {
        int d = next_tbl[last][i];
        seq_t nv = val * 7 + d;
        gen_len_rec(L, pos + 1, nv, d);
    }
}

/* Generate all valid base‑7 numbers of length L */
static void gen_len(idx_t L)
{
    if (L == 1) {
        for (int d = 1; d <= 6; ++d) {
            cache_push((seq_t)d);
        }
    } else {
        /* First digit must be 1, 5, or 6 (ascending). */
        const int firsts[3] = {1,5,6};
        for (int i = 0; i < 3; ++i) {
            int d = firsts[i];
            seq_t v = (seq_t)d;
            gen_len_rec(L, 1, v, d);
        }
    }
}

/* Public function returning the n-th term (a(1)=first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset */
    while (cache_cnt < n) {
        ++produced_len;
        gen_len(produced_len);
    }
    return cache[n-1];
}

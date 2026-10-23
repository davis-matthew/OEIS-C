/*
 * A164141  Number of binary strings of length n with equal numbers of 001 and
 *          010 substrings.
 * https://oeis.org/A164141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n having the same *
 *            number of 001 and 010 substrings. Computed by dynamic
 *            programming * over the last two bits and the difference * d =
 *            (#001) – (#010). For strings of length len the difference *
 *            satisfies |d| ≤ len–2, so the DP table for a given len has...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;   /* cache of a(n) values */
static size_t a_cap = 0;        /* capacity of a_cache */

static seq_t **cur = NULL;      /* DP tables for current length */
static seq_t **nxt = NULL;
static size_t cur_cap = 0;      /* allocated capacity per row */

/* Ensure a_cache can hold indices up to n */
static void ensure_a_cache(idx_t n)
{
    if ((idx_t)a_cap > n) return;
    size_t new_cap = a_cap == 0 ? 8 : a_cap;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    memset(a_cache + a_cap, 0, (new_cap - a_cap) * sizeof(seq_t));
    a_cap = new_cap;
}

/* Ensure DP arrays have capacity at least need */
static void ensure_dp_cap(size_t need)
{
    if (cur_cap >= need) return;
    size_t new_cap = cur_cap == 0 ? 8 : cur_cap;
    while (new_cap < need) new_cap <<= 1;
    cur_cap = new_cap;
    if (!cur) {
        cur = (seq_t **)malloc(4 * sizeof(seq_t *));
        nxt = (seq_t **)malloc(4 * sizeof(seq_t *));
    }
    for (int s = 0; s < 4; ++s) {
        cur[s] = (seq_t *)realloc(cur[s], cur_cap * sizeof(seq_t));
        nxt[s] = (seq_t *)realloc(nxt[s], cur_cap * sizeof(seq_t));
    }
}

/* Incrementally compute a(n) up to target n, filling a_cache. */
static void grow_to(idx_t n)
{
    static idx_t dp_len = 0;  /* last computed length (>=2) */

    if (dp_len >= n) return;

    /* Initialisation for length 2 */
    if (dp_len == 0) {
        size_t init_cap = 8;
        if (init_cap < 4) init_cap = 4;
        ensure_dp_cap(init_cap);
        for (int s = 0; s < 4; ++s)
            memset(cur[s], 0, cur_cap * sizeof(seq_t));
        /* All 4 length‑2 strings have diff 0 -> index 0 */
        cur[0][0] = (seq_t)1;
        cur[1][0] = (seq_t)1;
        cur[2][0] = (seq_t)1;
        cur[3][0] = (seq_t)1;

        ensure_a_cache(2);
        a_cache[0] = (seq_t)1;  /* empty string */
        a_cache[1] = (seq_t)2;  /* single‑bit strings */
        a_cache[2] = (seq_t)4;  /* length‑2 strings */
        dp_len = 2;
    }

    for (idx_t len = dp_len; len < n; ++len) {
        size_t size_curr = 2 * len - 3;   /* number of entries for current len */
        size_t size_next = size_curr + 2; /* for len+1 */

        ensure_dp_cap(size_next);
        for (int s = 0; s < 4; ++s)
            memset(nxt[s], 0, size_next * sizeof(seq_t));

        /* Transition from length len to len+1 */
        for (int s = 0; s < 4; ++s) {
            seq_t *row_cur = cur[s];
            for (size_t idx = 0; idx < size_curr; ++idx) {
                seq_t v = row_cur[idx];
                if (v == (seq_t)0) continue;

                /* Append bit 0 */
                int new_s0 = (s << 1) & 3;
                int delta0 = (s == 1) ? -1 : 0;    /* 010 occurs when s==01 */
                size_t new_idx0 = idx + 1 + (size_t)delta0;
                nxt[new_s0][new_idx0] += v;

                /* Append bit 1 */
                int new_s1 = ((s << 1) & 3) | 1;
                int delta1 = (s == 0) ? 1 : 0;     /* 001 occurs when s==00 */
                size_t new_idx1 = idx + 1 + (size_t)delta1;
                nxt[new_s1][new_idx1] += v;
            }
        }

        /* Sum counts with diff == 0 (center index) for all 4 states */
        size_t center = size_next / 2;
        seq_t total = (seq_t)0;
        for (int s = 0; s < 4; ++s)
            total += nxt[s][center];

        idx_t next_len = len + 1;
        ensure_a_cache(next_len);
        a_cache[next_len] = total;

        /* Swap cur and nxt pointers */
        for (int s = 0; s < 4; ++s) {
            seq_t *tmp = cur[s];
            cur[s] = nxt[s];
            nxt[s] = tmp;
        }

        dp_len = next_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    ensure_a_cache(n);
    grow_to(n);
    return a_cache[n];
}

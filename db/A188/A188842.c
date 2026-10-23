/*
 * A188842  Number of nX8 binary arrays without the pattern 0 1 diagonally or
 *          vertically.
 * https://oeis.org/A188842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 49, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188842 – Number of n×8 binary arrays without the pattern 0 1
   vertically or diagonally.  The following routine computes a(n) for
   all n ≥ 1.  The algorithm uses dynamic programming over rows:
   * Every row is represented as an 8‑bit pattern (0–255).
   * For a given current row u, the allowed next rows v are those for
     which no 0 is directly above a 1 vertically (u bit i must be 1
     whenever v bit i is 1) and no 0 is directly above a 1
     diagonally to the right (u bit i-1 must be 1 whenever v bit i
     is 1, for i>0).  These conditions can be tested in O(1).
   * The number of admissible n×8 matrices equals the number of
     directed paths of length n-1 in the graph of allowed transitions.
   * A memoised table stores the result for each requested n, so calls
     may appear in any order and each n is computed only once.
   * All arithmetic is performed with seq_t and idx_t as required by
     the build environment; no fixed-width types other than those
     guaranteed by the environment are used.  The algorithm is O(n)
     per call and well below the 2‑second limit for n up to 10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* adjacency information */
static unsigned char *adjacent[256];
static uint16_t adjacent_cnt[256]; /* need >255 for the all‑1 row */

/* memoisation of a(n) */
static seq_t *a_cache   = NULL;
static size_t a_cache_size = 0;
static idx_t max_computed = 0;

/* row DP tables */
static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;

/* Build the adjacency lists for 8‑bit rows */
static void build_adjacency(void)
{
    unsigned char *tmp = (unsigned char *)malloc(256 * sizeof(unsigned char));
    for (int u = 0; u < 256; ++u) {
        unsigned int us = u;                     /* u’s 8 bits */
        unsigned short cnt = 0;
        for (int v = 0; v < 256; ++v) {
            unsigned int vs = v;
            /* mask contains positions that must be 1 in u
               (vertical) or left neighbour (diagonal).   */
            unsigned int mask = vs | (vs >> 1);
            if ((~us & mask) == 0) {             /* v is allowed after u */
                tmp[cnt++] = (unsigned char)v;
            }
        }
        adjacent_cnt[u] = cnt;
        if (cnt == 0) {
            adjacent[u] = NULL;
        } else {
            adjacent[u] = (unsigned char *)malloc(cnt * sizeof(unsigned char));
            memcpy(adjacent[u], tmp, cnt);
        }
    }
    free(tmp);
}

/* Initialise DP tables (once) */
static void init_state_arrays(void)
{
    dp_cur = (seq_t *)malloc(256 * sizeof(seq_t));
    dp_nxt = (seq_t *)malloc(256 * sizeof(seq_t));
}

/* Ensure that a_cache[1..n] is ready */
static void ensure_cache(idx_t n)
{
    if (max_computed >= n) return;

    size_t needed = (size_t)n + 1;
    if (a_cache_size < needed) {
        seq_t *new_cache = (seq_t *)realloc(a_cache, needed * sizeof(seq_t));
        if (!new_cache) abort();
        a_cache   = new_cache;
        a_cache_size = needed;
    }

    /* initialise for a(1) if necessary */
    if (max_computed == 0) {
        for (int u = 0; u < 256; ++u) dp_cur[u] = 1;
        seq_t sum = 0;
        for (int u = 0; u < 256; ++u) sum += dp_cur[u];
        a_cache[1] = sum;
        max_computed = 1;
    }

    /* compute intermediate terms up to n */
    for (idx_t step = max_computed + 1; step <= n; ++step) {
        memset(dp_nxt, 0, 256 * sizeof(seq_t));
        for (int u = 0; u < 256; ++u) {
            seq_t val = dp_cur[u];
            if (val == 0) continue;
            unsigned char *to  = adjacent[u];
            uint16_t   cnt     = adjacent_cnt[u];
            for (int i = 0; i < (int)cnt; ++i) {
                dp_nxt[to[i]] += val;
            }
        }
        seq_t sum = 0;
        for (int v = 0; v < 256; ++v) sum += dp_nxt[v];
        a_cache[step] = sum;
        /* swap dp_cur and dp_nxt for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }
    max_computed = n;
}

/* public interface required by the exercise */
seq_t a(idx_t n)
{
    static bool initialised = false;
    if (!initialised) {
        build_adjacency();
        init_state_arrays();
        initialised = true;
    }
    if (n == 0) return 0;
    ensure_cache(n);
    return a_cache[n];
}

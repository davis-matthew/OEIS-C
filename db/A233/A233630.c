/*
 * A233630  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock having
 *          the sum of the absolute values of the edge differences equal to 6.
 * https://oeis.org/A233630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   absdiff, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A233630
   Formula: a(n) = number of (n+1)×4 arrays of digits 0..3
   satisfying that for every 2×2 subblock the sum of the absolute
   differences of its four edges equals 6. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/absdiff.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ---------- adjacency lists ---------- */
static bool built_adj = false;
static uint8_t  nxt_list[256][256];   /* nxt_list[i][k] = j (i→j) */
static uint8_t  nxt_cnt [256];        /* number of outgoing edges from i */

/* Build the graph once */
static void build_graph(void)
{
    if (built_adj) return;
    for (int a = 0; a < 256; ++a) {
        int r[4];
        for (int t = 0; t < 4; ++t) r[t] = (a >> (2 * t)) & 3;
        for (int b = 0; b < 256; ++b) {
            int s[4];
            for (int t = 0; t < 4; ++t) s[t] = (b >> (2 * t)) & 3;
            bool ok = true;
            for (int col = 0; col < 3 && ok; ++col) {
                int dtop    = absdiff(r[col],     r[col + 1]);
                int dleft   = absdiff(r[col],     s[col]);
                int dbottom = absdiff(s[col],     s[col + 1]);
                int dright  = absdiff(r[col + 1], s[col + 1]);
                if (dtop + dleft + dbottom + dright != 6) ok = false;
            }
            if (ok) {
                nxt_list[a][nxt_cnt[a]++] = (uint8_t)b;
            }
        }
    }
    built_adj = true;
}

/* ---------- DP cache ---------- */
static idx_t          cache_max_t = 1;   /* highest t computed (>=1) */
static seq_t          *dp_cur = NULL;    /* dp for current t rows */
static seq_t          *dp_next = NULL;   /* temporary dp */
static seq_t          *sums = NULL;       /* sums[t] = #arrays with t rows */
static size_t         sums_size = 0;      /* allocated size of sums */

/* Initialise DP structures */
static void ensure_initialized(void)
{
    if (dp_cur != NULL) return;          /* already done */

    /* Build graph once */
    build_graph();

    /* Allocate dp arrays */
    dp_cur  = seq_alloc(256);
    dp_next = seq_alloc(256);
    if (!dp_cur || !dp_next) abort();    /* out‑of‑memory */

    /* t = 1 : any of 256 rows can start */
    for (int i = 0; i < 256; ++i) dp_cur[i] = (seq_t)1;
    cache_max_t = 1;

    /* Allocate sums array with room for a few entries */
    sums_size = 5;
    sums = seq_alloc(sums_size);
    if (!sums) abort();
    sums[1] = (seq_t)256;                /* 1‑row arrays (not part of OEIS) */
}

/* Extend cache up to desired_t (>= cache_max_t) */
static void extend_cache_to(idx_t desired_t)
{
    if (desired_t <= cache_max_t) return;

    /* Ensure sums array large enough */
    if ((idx_t)(desired_t + 1) > sums_size) {
        size_t new_size = desired_t + 1;
        seq_t *tmp = seq_realloc(sums, sums_size, new_size);
        if (!tmp) abort();
        sums = tmp;
        sums_size = new_size;
    }

    /* Compute successive t values */
    for (idx_t t = cache_max_t + 1; t <= desired_t; ++t) {
        /* Zero dp_next */
        for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;

        seq_t step_sum = (seq_t)0;
        for (int i = 0; i < 256; ++i) {
            seq_t vi = dp_cur[i];
            if (vi == (seq_t)0) continue;
            uint8_t deg = nxt_cnt[i];
            if (deg == 0) continue;
            step_sum += vi * (seq_t)deg;
            for (int k = 0; k < deg; ++k) {
                int j = nxt_list[i][k];
                dp_next[j] += vi;
            }
        }
        sums[t] = step_sum;             /* total arrays of t rows */

        /* Prepare for next step */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
        cache_max_t = t;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset 1 */
    ensure_initialized();

    idx_t needed_t = n + 1;             /* number of rows needed */
    extend_cache_to(needed_t);
    return sums[needed_t];
}

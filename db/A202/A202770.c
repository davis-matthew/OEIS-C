/*
 * A202770  Number of (n+2)X3 binary arrays avoiding patterns 000 and 011 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 30, u64 n <= 70, u128 n <= 149,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A202770: number of (n+2)×3 binary arrays that avoid the
 * 3‑cell patterns 000 and 011 in every row, every column and every
 * northwest–to–southeast diagonal.  Computed by dynamic programming
 * with a state consisting of the last two rows.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* 8‑bit masks for 3‑bit rows.  A row mask is valid iff
 * it is not 000 (0) and not 011 (6).  Bit 2 is the leftmost
 * column, bit 0 the rightmost. */
static const unsigned char validRow[8] = {
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    0, /* 011 */
    1, /* 100 */
    1, /* 101 */
    1, /* 110 */
    1  /* 111 */
};

/* Transition table: trans[s][k] gives a reachable successor state
 * (encoded as (new_second_row << 3) | new_row) when a new row
 * succeeds the pair represented by state s.  trans_sz[s] is the
 * number of reachable successors for s. */
static unsigned char trans[64][8];
static unsigned char trans_sz[64];

/* Build the transition table once. */
static void init_transitions(void)
{
    for (int s = 0; s < 64; ++s) {
        unsigned char r1 = (unsigned char)(s >> 3); /* second‑last row */
        unsigned char r2 = (unsigned char)(s & 7);  /* last row */

        if (!validRow[r1] || !validRow[r2]) {
            trans_sz[s] = 0;
            continue;
        }

        unsigned char sz = 0;
        for (unsigned char r3 = 0; r3 < 8; ++r3) {
            if (!validRow[r3])
                continue;

            /* Column constraints for each of the three columns. */
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                unsigned char b1 = (r1 >> j) & 1;
                unsigned char b2 = (r2 >> j) & 1;
                unsigned char b3 = (r3 >> j) & 1;
                if ((b1 | b2 | b3) == 0)          /* 000 */
                    ok = false;
                if (b1 == 0 && b2 == 1 && b3 == 1) /* 011 */
                    ok = false;
            }

            /* NW‑to‑SE diagonal triple: (r1[2], r2[1], r3[0]). */
            if (ok) {
                unsigned char b1 = (r1 >> 2) & 1; /* leftmost of r1 */
                unsigned char b2 = (r2 >> 1) & 1; /* middle of r2  */
                unsigned char b3 = (r3 >> 0) & 1; /* rightmost of r3 */
                if ((b1 | b2 | b3) == 0)          /* 000 */
                    ok = false;
                if (b1 == 0 && b2 == 1 && b3 == 1) /* 011 */
                    ok = false;
            }

            if (ok)
                trans[s][sz++] = (unsigned char)((r2 << 3) | r3);
        }
        trans_sz[s] = sz;
    }
}

/* External‑linkage entry point. */
seq_t a(idx_t n)
{
    static bool init = false;
    static idx_t computed_n = 0;       /* largest n already computed   */
    static seq_t *dp_cur  = NULL;      /* counts for state after height */
    static seq_t *dp_next = NULL;      /* temporary for next height    */
    static seq_t *cache   = NULL;      /* cache of a(1)..a(max) values */
    static size_t cache_len = 0;       /* allocated length of cache    */

    if (!init) {
        init_transitions();

        dp_cur  = seq_alloc_new_array(64);
        dp_next = seq_alloc_new_array(64);
        memset(dp_cur, 0, 64 * sizeof(seq_t));
        memset(dp_next, 0, 64 * sizeof(seq_t));

        /* Initial arrays of height 2 (two rows).  Each valid pair of
         * rows counts as one configuration. */
        for (int s = 0; s < 64; ++s) {
            unsigned char r1 = (unsigned char)(s >> 3);
            unsigned char r2 = (unsigned char)(s & 7);
            if (validRow[r1] && validRow[r2])
                dp_cur[s] = (seq_t)1;
        }
        init = true;
    }

    /* Allocate or grow cache if necessary. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;      /* indices 0..n     */
        seq_t *new_cache = seq_alloc_new_array(new_len);
        if (cache) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            seq_free_array(cache);
        }
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute new values up to requested n. */
    for (idx_t k = computed_n + 1; k <= n; ++k) {
        /* Transition: add one more row to reach height k+2. */
        memset(dp_next, 0, 64 * sizeof(seq_t));
        for (int s = 0; s < 64; ++s) {
            seq_t cnt = dp_cur[s];
            if (!cnt)
                continue;
            unsigned char sz = trans_sz[s];
            for (unsigned char idx = 0; idx < sz; ++idx) {
                unsigned char t = trans[s][idx];
                dp_next[t] += cnt;
            }
        }
        /* Swap dp_cur and dp_next for next iteration. */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* Sum over all states to obtain a(k). */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < 64; ++s)
            sum += dp_cur[s];
        cache[k] = sum;
        computed_n = k;
    }

    return cache[n];
}

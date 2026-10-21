/*
 * A083572  Smaller of two consecutive star numbers (A000567) whose sum is a
 *          square.
 * https://oeis.org/A083572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   advance_to_next_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083572 – Smaller of two consecutive star numbers (S_k = 3k²–2k)
    whose sum is a perfect square.
    The k‑values satisfy 6k²+2k+1 = m².
    Setting d = 6k+1 gives the negative Pell equation
        d² – 6m² = –5.
    Its fundamental solutions are (1,1) and (7,3); all other
    solutions are produced by the recurrence
        d' = 5d + 12m
        m' = 2d + 5m
    (d,m) is accepted only when d ≡ 1 (mod 6).  The sequence a(n)
    contains 3k²–2k for each such k in increasing order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/advance_to_next_valid.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;     /* 0‑indexed cache of a(1)…a(max_n) */
    static idx_t  cache_len = 0;    /* number of terms stored         */
    static idx_t  cache_cap = 0;    /* capacity of allocated cache    */
    static seq_t  d1 = 0, m1 = 0;   /* branch 1 (starting (1,1))     */
    static seq_t  d2 = 0, m2 = 0;   /* branch 2 (starting (7,3))     */
    static bool   initialized = false;

    if (!initialized) {
        /* initialise first cache and branches */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        d1 = (seq_t)1;   m1 = (seq_t)1;   /* (1,1)  → k=0 → S_0=0   */
        d2 = (seq_t)7;   m2 = (seq_t)3;   /* (7,3)  → k=1 → S_1=1   */
        initialized = true;
    }

    /* grow cache to at least n terms */
    while (cache_len < n) {
        /* current valid solutions in each branch, if any */
        bool ok1 = false, ok2 = false;
        seq_t k1_ts = 0, k2_ts = 0;
        seq_t val1 = 0, val2 = 0;

        if (d1 % (seq_t)6 == (seq_t)1) {
            k1_ts = (d1 - (seq_t)1) / (seq_t)6;
            val1   = (seq_t)3 * k1_ts * k1_ts - (seq_t)2 * k1_ts;
            ok1 = true;
        }
        if (d2 % (seq_t)6 == (seq_t)1) {
            k2_ts = (d2 - (seq_t)1) / (seq_t)6;
            val2   = (seq_t)3 * k2_ts * k2_ts - (seq_t)2 * k2_ts;
            ok2 = true;
        }

        /* pick the branch with the smaller k (equivalently smaller val) */
        bool pick_branch1;
        if (ok1 && (!ok2 || k1_ts <= k2_ts))
            pick_branch1 = true;
        else if (ok2)
            pick_branch1 = false;
        else
            /* should never happen for valid initialisation */
            return 0;

        seq_t new_val;
        if (pick_branch1) {
            new_val = val1;
            advance_to_next_valid(&d1, &m1);
        } else {
            new_val = val2;
            advance_to_next_valid(&d2, &m2);
        }

        /* ensure cache has space for the new term */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = new_val;
        cache_len++;
    }

    return cache[n-1];
}

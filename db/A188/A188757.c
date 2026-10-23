/*
 * A188757  Number of nX3 binary arrays without the pattern 0 0 1 vertically
 *          or horizontally.
 * https://oeis.org/A188757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of n×3 binary arrays avoiding the pattern 0 0 1
   vertically or horizontally.  The DP keeps the last two rows (3 bits
   each) as a state, hence 64 possible states.  Transition to a new row
   checks the horizontal condition (row ≠ 001) and the vertical condition
   (no column has 0,0,1 in consecutive rows).  The computed values are
   memoised in a static cache so that arbitrary call order and multiple
   invocations are efficient. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Early out for indices below the OEIS offset */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static state --------------------------------------------- */
    static size_t       cache_cap     = 0;      /* capacity of cache */
    static seq_t *      cache         = NULL;   /* cached values a(1)…a(max) */
    static idx_t        max_n_computed = 0;    /* highest n computed so far */
    static seq_t        dp_prev[64] = {0};     /* counts for length cur-1    */
    static seq_t        dp_next[64] = {0};     /* counts for length cur      */
    static int          initialised   = 0;     /* 0 → dp_prev not yet set    */

    /* Grow the cache if necessary -------------------------------- */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n;
        /* realloc returns void* – cast to seq_t* for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {          /* allocation failure – shouldn't happen in tests */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return previously computed value --------------------------- */
    if (n <= max_n_computed) {
        return cache[n - 1];
    }

    /* Initialise the computation for n = 1 ------------------------ */
    if (!initialised) {
        cache[0] = (seq_t)7;          /* all 3‑bit rows except 001 */
        max_n_computed = 1;
        /* dp_prev remains all zero; it will be populated when n ≥ 2 */
        initialised = 1;
    }

    /* Build results up to the requested n ------------------------ */
    while (max_n_computed < n) {
        /* ------------------------------------------ n = 2 special case */
        if (max_n_computed == 1) {
            /* Reset dp_prev */
            for (int i = 0; i < 64; ++i) dp_prev[i] = (seq_t)0;

            /* Enumerate all pairs of valid rows (r1,r2) */
            for (int r1 = 0; r1 < 8; ++r1) {
                if (r1 == 1) continue;          /* exclude 001 */
                for (int r2 = 0; r2 < 8; ++r2) {
                    if (r2 == 1) continue;
                    int id = (r1 << 3) | r2;
                    dp_prev[id] = (seq_t)1;
                }
            }

            /* Sum to get a(2) */
            seq_t sum = (seq_t)0;
            for (int i = 0; i < 64; ++i) sum += dp_prev[i];
            cache[1] = sum;

            max_n_computed = 2;
            continue;
        }

        /* ------------------------------------------ Transition step */
        /* Zero dp_next */
        for (int i = 0; i < 64; ++i) dp_next[i] = (seq_t)0;

        /* For each previous state (prev2, prev1) */
        for (int state = 0; state < 64; ++state) {
            seq_t count = dp_prev[state];
            if (count == (seq_t)0) continue;

            int p2 = state >> 3;   /* older row */
            int p1 = state & 7;    /* newer row */

            /* Try each candidate new row r */
            for (int r = 0; r < 8; ++r) {
                if (r == 1) continue; /* horizontal restriction */

                /* Check vertical restriction */
                int ok = 1;
                for (int j = 0; j < 3; ++j) {
                    int b2 = (p2 >> j) & 1;
                    int b1 = (p1 >> j) & 1;
                    int cr = (r   >> j) & 1;
                    if (b2 == 0 && b1 == 0 && cr == 1) {
                        ok = 0;
                        break;
                    }
                }
                if (!ok) continue;

                int new_id = (p1 << 3) | r;
                dp_next[new_id] += count;
            }
        }

        /* Sum all states to get a(cur) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 64; ++i) sum += dp_next[i];
        cache[max_n_computed] = sum;

        /* Prepare for next iteration */
        for (int i = 0; i < 64; ++i) dp_prev[i] = dp_next[i];

        max_n_computed++;
    }

    return cache[n - 1];
}

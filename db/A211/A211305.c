/*
 * A211305  a(n) = number of n-lettered words in the alphabet {1, 2, 3, 4}
 *          with as many occurrences of the substring (consecutive subword)
 *          [1, 1] as of [2, 3].
 * https://oeis.org/A211305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211305: number of n-letter words over {1,2,3,4}
   with as many occurrences of the substring [1,1] as of [2,3]. */

#include <stdlib.h>

#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) -> seq_t*   */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*)              */

/* static cache of computed terms a(0), a(1), … */
static seq_t *terms = NULL;          /* array of seq_t values */
static size_t terms_cap = 0;         /* current capacity of terms */

/* transition DP array for the current length */
static seq_t **dp_curr = NULL;       /* dp_curr[4][2*n+1]  */
static size_t dp_cur_n = 0;          /* length of dp_curr    */

/* --------------------------------------------------------------------- */
/*  Initialise the cache for the base cases and for words of length 1.  */
static void init_sequence(void)
{
    /* base terms a(0)=1, a(1)=4 */
    terms_cap = 2;
    terms = alloc_seq_array(terms_cap);
    terms[0] = (seq_t)1;           /* empty word */
    terms[1] = (seq_t)4;           /* any single letter */

    /* DP for words of length 1 – difference is always 0. */
    dp_cur_n = 1;
    dp_curr = (seq_t **)malloc(sizeof(seq_t *) * 4);
    size_t sz = 2 * dp_cur_n + 1;            /* sz = 3 */
    for (int c = 0; c < 4; ++c) {
        dp_curr[c] = alloc_seq_array(sz);
        for (size_t i = 0; i < sz; ++i) dp_curr[c][i] = (seq_t)0;
        dp_curr[c][dp_cur_n] = (seq_t)1;     /* one word ending with each letter */
    }
}

/* --------------------------------------------------------------------- */
/*  Extend the cache up to index n (if necessary).                       */
static void ensure_capacity(idx_t n)
{
    if (!terms) init_sequence();

    while ((idx_t)terms_cap <= n) {
        /* Prepare arrays for the next word length. */
        size_t next_n   = dp_cur_n + 1;
        size_t next_sz  = 2 * next_n + 1;     /* size of diff array */
        seq_t **dp_next = (seq_t **)malloc(sizeof(seq_t *) * 4);
        for (int c = 0; c < 4; ++c) {
            dp_next[c] = alloc_seq_array(next_sz);
            for (size_t i = 0; i < next_sz; ++i) dp_next[c][i] = (seq_t)0;
        }

        /* Transition from dp_curr to dp_next. */
        size_t cur_sz = 2 * dp_cur_n + 1;
        for (int prev = 0; prev < 4; ++prev) {
            for (size_t idx = 0; idx < cur_sz; ++idx) {
                seq_t val = dp_curr[prev][idx];
                if (val == (seq_t)0) continue;

                int diff = (int)idx - (int)dp_cur_n;   /* current difference */

                for (int cur = 0; cur < 4; ++cur) {
                    int delta = 0;
                    if (prev == 0 && cur == 0) delta =  1;   /* "11" */
                    if (prev == 1 && cur == 2) delta = -1;   /* "23" */
                    int new_diff = diff + delta;
                    size_t new_idx = (size_t)(new_diff + (int)next_n);
                    dp_next[cur][new_idx] += val;
                }
            }
        }

        /* Compute a(next_n) – sum of states with difference 0. */
        seq_t a_next = (seq_t)0;
        for (int c = 0; c < 4; ++c) a_next += dp_next[c][next_n];

        /* Grow the terms array to store the new value. */
        seq_t *new_terms = alloc_seq_array(terms_cap + 1);
        for (size_t i = 0; i < terms_cap; ++i) new_terms[i] = terms[i];
        new_terms[terms_cap] = a_next;
        free_seq_array(terms);
        terms = new_terms;
        ++terms_cap;

        /* Free the old DP arrays and adopt the new ones. */
        for (int c = 0; c < 4; ++c) free_seq_array(dp_curr[c]);
        free(dp_curr);
        dp_curr = dp_next;
        dp_cur_n = next_n;
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the judge.                              */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return terms[n];
}

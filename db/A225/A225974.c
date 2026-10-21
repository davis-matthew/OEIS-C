/*
 * A225974  Multiplicative persistence with squares of decimal digits:
 *          smallest number such that the number of iterations of "multiply
 *          digits squared" needed to reach 0 or 1 equals n.
 * https://oeis.org/A225974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   persistence_cnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A225974: a(n) is the smallest integer whose
 * “multiply digits square” process takes exactly n steps to reach 0 or 1.
 * The routine uses a modest brute‑force search, remembering the first
 * integer found for each persistence value.  It will return 0 if
 * the desired persistence has not been found within the pre‑defined limit
 * (10 000 000).  The algorithm is deterministic and works for any
 * seq_t type that supports the usual arithmetic operators. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/persistence_cnt.h"

/* Keep the minimal value for each persistence in a growing table. */
static seq_t *min_val      = NULL;   /* min_val[p] = smallest k with persistence p */
static bool   *has_val     = NULL;   /* has_val[p] = true if min_val[p] is set */
static size_t   cap        = 0;       /* allocated size of the two arrays */
static idx_t    processed  = 0;       /* maximum number processed so far */
static const idx_t SEARCH_LIMIT = 10000000;  /* 10 million – enough for all small n */

/* The public function required by the exercise. */
seq_t a(idx_t n)
{
    /* Initialise the data structures on the first call. */
    if (min_val == NULL) {
        cap = 32;
        min_val = (seq_t*)malloc(cap * sizeof(seq_t));
        has_val = (bool*)malloc(cap * sizeof(bool));
        if (!min_val || !has_val)   /* allocation failed */
            return (seq_t)0;
        for (size_t i = 0; i < cap; ++i) {
            min_val[i] = (seq_t)0;   /* dummy, overwritten when set */
            has_val[i] = false;
        }
        /* persistence 0 is reached by 0 and 1, choose the smallest. */
        min_val[0] = 0;
        has_val[0] = true;
    }

    /* Ensure the table is large enough to hold the requested index. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        seq_t *tmp_val = (seq_t*)realloc(min_val, new_cap * sizeof(seq_t));
        bool   *tmp_has = (bool*)realloc(has_val, new_cap * sizeof(bool));
        if (!tmp_val || !tmp_has) {   /* realloc failed */
            free(min_val);
            free(has_val);
            min_val = NULL;
            has_val = NULL;
            return (seq_t)0;
        }
        /* initialise newly allocated slots */
        for (size_t i = cap; i < new_cap; ++i) {
            tmp_val[i] = (seq_t)0;
            tmp_has[i] = false;
        }
        min_val = tmp_val;
        has_val = tmp_has;
        cap = new_cap;
    }

    /* If we already know the answer for this n, return it immediately. */
    if (has_val[(size_t)n])
        return min_val[(size_t)n];

    /* Perform brute‑force search until we either find the persistence
     * or reach the global search limit. */
    for (idx_t k = processed; k <= SEARCH_LIMIT; ++k) {
        seq_t num = (seq_t)k;
        int p = persistence_cnt(num);
        if (p >= 0) {
            /* Grow table if a higher persistence is found. */
            if ((size_t)p >= cap) {
                size_t new_cap = cap;
                while ((size_t)p >= new_cap)
                    new_cap *= 2;
                seq_t *tmp_val = (seq_t*)realloc(min_val, new_cap * sizeof(seq_t));
                bool   *tmp_has = (bool*)realloc(has_val, new_cap * sizeof(bool));
                if (!tmp_val || !tmp_has) {
                    free(min_val);
                    free(has_val);
                    min_val = NULL;
                    has_val = NULL;
                    return (seq_t)0;
                }
                for (size_t i = cap; i < new_cap; ++i) {
                    tmp_val[i] = (seq_t)0;
                    tmp_has[i] = false;
                }
                min_val = tmp_val;
                has_val = tmp_has;
                cap = new_cap;
            }
            /* Record the first (smallest) number for this persistence. */
            if (!has_val[(size_t)p]) {
                has_val[(size_t)p] = true;
                min_val[(size_t)p]  = num;
                if (has_val[(size_t)n])      /* the target found */
                    return min_val[(size_t)n];
            }
        }
    }

    /* Update the processed marker to avoid re‑doing work on subsequent calls. */
    processed = SEARCH_LIMIT + 1;

    /* Either the persistence exists (unlikely beyond our search limit)
     * or it does not.  Return 0 if not found. */
    return has_val[(size_t)n] ? min_val[(size_t)n] : (seq_t)0;
}

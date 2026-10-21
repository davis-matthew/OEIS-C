/*
 * A090428  Numbers that are k-gons for two or more k's, where k = 3, 4, 5,
 *          ..., 16.
 * https://oeis.org/A090428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are k‑gonal for two or more k ∈ {3,…,16}
 * Range:     u8 n <= 28, u16 n <= 246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates each k‑gonal sequence
   (P(k,m) = m[(k‑2)m – (k‑4)]/2) incrementally using the
   recurrence:
        P(k,1) = 1,
        increment(k,1) = k – 1,
        P(k,m+1) = P(k,m) + increment(k,m),
        increment(k,m+1) = increment(k,m) + (k – 2).
   While iterating, the algorithm extracts the smallest current
   value among all k, checks how many k produce that value,
   records it if at least two, and advances those k. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* k ranges from 3 to 16 inclusive */
    enum { NUM_K = 14 };
    static int k_vals[NUM_K];
    static idx_t m_idx[NUM_K];
    static seq_t curr_val[NUM_K];
    static seq_t inc[NUM_K];
    static bool init = false;

    /* storage of the sequence of interest */
    static seq_t *good = NULL;
    static size_t good_len = 0;
    static size_t good_cap = 0;

    if (!init) {
        for (int i = 0; i < NUM_K; ++i) {
            k_vals[i] = 3 + i;           /* k */
            m_idx[i]  = 1;               /* current index m */
            curr_val[i] = (seq_t)1;      /* P(k,1) = 1 for all k */
            inc[i] = (seq_t)(k_vals[i] - 1); /* first increment */
        }
        init = true;
    }

    /* produce terms until we have at least n of them */
    while (good_len < n) {
        /* find the smallest current value */
        seq_t minv = curr_val[0];
        for (int i = 1; i < NUM_K; ++i)
            if (curr_val[i] < minv)
                minv = curr_val[i];

        /* which k's achieve this minimum */
        int dup_idx[NUM_K];
        int dup_cnt = 0;
        for (int i = 0; i < NUM_K; ++i)
            if (curr_val[i] == minv)
                dup_idx[dup_cnt++] = i;

        /* advance each k that contributed the minimum */
        for (int d = 0; d < dup_cnt; ++d) {
            int i = dup_idx[d];
            /* advance m */
            ++m_idx[i];
            /* update value and increment using the recurrence */
            curr_val[i] += inc[i];
            inc[i] += (seq_t)(k_vals[i] - 2);
        }

        /* store the value if it appears for two or more k's */
        if (dup_cnt >= 2) {
            if (good_len == good_cap) {
                size_t new_cap = good_cap == 0 ? 16 : good_cap * 2;
                seq_t *tmp = seq_realloc(good, good_cap, new_cap);
                good = tmp;
                good_cap = new_cap;
            }
            good[good_len++] = minv;
        }
    }

    return good[n - 1];
}

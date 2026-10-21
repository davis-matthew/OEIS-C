/*
 * A081828  Numbers n such that 2 successive terms of Hofstadter Q-sequence
 *          are the same.
 * https://oeis.org/A081828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 189, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   ensure_seq_cap_idx_t, ensure_seq_cap_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081828
   Numbers n such that Q(n) = Q(n+1), where Q is the Hofstadter Q–sequence:
       Q(1) = Q(2) = 1
       Q(k) = Q(k - Q(k-1)) + Q(k - Q(k-2))   for k > 2
   a(n) is the n-th index where a consecutive pair of Q–terms is equal.
*/

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free, realloc */
#include "../utils/ensure_seq_cap_idx_t.h"
#include "../utils/ensure_seq_cap_seq_t.h"

/* Forward declarations for dynamic array helpers */
static void ensure_seq_cap_seq_t(size_t *cap, size_t need, seq_t **ptr);
static void ensure_seq_cap_idx_t(size_t *cap, size_t need, idx_t **ptr);

/* The actual sequence function */
seq_t a(idx_t n)
{
    /* Static caches: Q values (1‑based, Q[0] unused) and equals indices */
    static seq_t *Q        = NULL;
    static size_t Q_len       = 0;   /* number of elements stored (including dummy) */
    static size_t Q_cap       = 0;   /* capacity of Q array */

    static idx_t *eq = NULL;
    static size_t eq_len = 0;   /* number of equal indices stored */
    static size_t eq_cap = 0;   /* capacity of eq array */

    /* ----- 1. Initialise caches on first call ----- */
    if (!Q_cap) {
        /* Allocate initial capacity for Q (at least 8 elements) */
        Q_cap = 8;
        Q = (seq_t *)malloc(Q_cap * sizeof(seq_t));
        if (!Q) return 0;          /* allocation failed – return 0 as sentinel */

        /* Q[0] is unused, set to 0 for safety */
        Q[0] = 0;
        Q_len = 1;

        /* Insert first two values */
        ensure_seq_cap_seq_t(&Q_cap, Q_len + 2, &Q);
        Q[1] = 1;  /* Q(1) */
        Q[2] = 1;  /* Q(2) */
        Q_len = 3;  /* now indices 0,1,2 are stored */

        /* Prepare eq cache */
        eq_cap = 8;
        eq = (idx_t *)malloc(eq_cap * sizeof(idx_t));
        if (!eq) return 0;
        eq[0] = 1;  /* Q(1)=Q(2) */
        eq_len = 1;
    }

    /* ----- 2. Grow Q and eq until we have at least n equalities ----- */
    while (eq_len < (size_t)n) {
        /* Next index to compute */
        idx_t i = (idx_t)Q_len;   /* Q_len is current number of stored elements */

        /* Compute Q(i) = Q(i-Q[i-1]) + Q(i-Q[i-2]) */
        idx_t idx1 = i - (idx_t)Q[i-1];
        idx_t idx2 = i - (idx_t)Q[i-2];

        seq_t val = Q[idx1] + Q[idx2];

        /* Append val to Q */
        ensure_seq_cap_seq_t(&Q_cap, Q_len + 1, &Q);
        Q[Q_len] = val;
        Q_len++;

        /* If the new value equals the previous one, record the index (i-1) */
        if (Q[i-1] == val) {
            ensure_seq_cap_idx_t(&eq_cap, eq_len + 1, &eq);
            eq[eq_len] = i - 1;
            eq_len++;
        }
    }

    /* eq array is 0‑based; a(n) is the (n‑1)‑th stored index */
    return eq[n-1];
}

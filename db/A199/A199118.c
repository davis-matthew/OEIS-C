/*
 * A199118  Number of partitions of n into terms of (1,3)-Ulam sequence, cf.
 *          A002859.
 * https://oeis.org/A199118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP for partitions of n into 1‑3‑Ulam sequence terms (A002859). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_free_array.h"

/* Trivial allocator helpers; these work for both built‑in types and
   arbitrary‑precision integer classes. */
static seq_t *seq_alloc_array(size_t n) {
    /* Allocate an array initialized to 0.  This is safe for any seq_t
       that is trivially default constructible (built‑in integers) or
       implements a default constructor (arbitrary‑precision types). */
    seq_t *p = (seq_t *)malloc(n * sizeof(seq_t));
    if (!p) return NULL;
    for (size_t i = 0; i < n; ++i) p[i] = (seq_t)0;
    return p;
}

/* ------------------------------------------------------------------
   Storage for the Ulam sequence (A002859).  It is rebuilt lazily on
   demand.  Only terms ≤ n are guaranteed to be present.  The
   sequence is started: 1, 3.  The generation routine searches for
   the next smallest integer that can be written as a sum of two distinct
   earlier terms in exactly one way.
   ------------------------------------------------------------------ */
static idx_t *ulam_seq  = NULL;      /* array of Ulam numbers */
static size_t ulam_len   = 0;        /* number of generated terms   */
static size_t ulam_cap   = 0;        /* allocated capacity */

/* Increase capacity of the ulam_seq array. */
static void
grow_ulam(size_t min_cap) {
    if (ulam_cap >= min_cap) return;
    size_t new_cap = ulam_cap ? ulam_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    idx_t *tmp = (idx_t *)realloc(ulam_seq, new_cap * sizeof(idx_t));
    if (tmp) {
        ulam_seq = tmp;
        ulam_cap = new_cap;
    } /* on failure we silently keep the old pointer */
}

/* Ensure that all Ulam numbers ≤ n are present in ulam_seq. */
static void
ensure_ulam_up_to(idx_t n) {
    if (!ulam_seq) {
        /* first initialization */
        grow_ulam(16);
        ulam_seq[0] = 1;
        ulam_seq[1] = 3;
        ulam_len = 2;
    }

    while (ulam_len > 0 && ulam_seq[ulam_len - 1] <= n) {
        idx_t cand = ulam_seq[ulam_len - 1] + 1;
        while (1) {
            size_t i = 0, j = ulam_len - 1;
            size_t count = 0;
            while (i < j) {
                idx_t sum = ulam_seq[i] + ulam_seq[j];
                if (sum == cand) {
                    ++count;
                    ++i; --j;
                    if (count > 1) break;
                } else if (sum < cand) {
                    ++i;
                } else {
                    --j;
                }
            }
            if (count == 1) {
                /* unique representation found */
                if (ulam_len == ulam_cap) grow_ulam(ulam_cap << 1);
                ulam_seq[ulam_len++] = cand;
                break;
            }
            ++cand;
        }
    }
}

/* ------------------------------------------------------------------
   Main function required by the framework.
   ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    /* Generate all Ulam numbers needed for this n. */
    ensure_ulam_up_to(n);

    /* Allocate DP array for 0..n inclusive.  Since seq_alloc_array
       zero‑initializes, we only need to set the base case. */
    seq_t *dp = seq_alloc_array((size_t)n + 1);
    if (!dp) return (seq_t)0;      /* allocation failure */

    dp[0] = (seq_t)1;

    /* Standard coin‑change DP: for each allowed part, add its
       contribution to all larger sums. */
    for (size_t ui = 0; ui < ulam_len; ++ui) {
        idx_t part = ulam_seq[ui];
        if (part > n) break;
        for (idx_t s = part; s <= n; ++s) {
            dp[(size_t)s] += dp[(size_t)(s - part)];
        }
    }

    seq_t result = dp[(size_t)n];
    seq_free_array(dp);
    return result;
}

/*
 * A080590  a(1)=1; for n>1, a(n)=a(n-1)+3 if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A080590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A080590: a(1)=1; for n>1, a(n)=a(n‑1)+3 if n already appears
   in the sequence, otherwise a(n‑1)+4.  The function keeps a memoised
   table of previously computed values and a boolean array that marks
   which numbers have appeared as a term.  The array is resized as needed
   and can handle the arbitrary‑precision build because only values up to
   the maximum index requested are indexed. */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration (needed for the helper) */
static void enlarge_seen_idx(size_t new_cap);

/* Global static state kept between calls */
static seq_t *seq          = NULL;   /* sequence values (1‑based)          */
static size_t seq_cap      = 0;       /* allocated capacity of seq array   */
static bool  *seen_idx     = NULL;   /* seen_idx[i] = true if i appeared  */
static size_t seen_cap     = 0;       /* allocated capacity of seen_idx    */
static idx_t  computed = 0;          /* largest n computed so far         */

/* Helper: increase seen_idx capacity to at least new_cap (1‑based) */
static void enlarge_seen_idx(size_t new_cap) {
    if (new_cap > seen_cap) {
        size_t old_cap = seen_cap;
        seen_cap = new_cap;
        seen_idx = (bool *)realloc(seen_idx, seen_cap * sizeof(bool));
        for (size_t i = old_cap; i < seen_cap; ++i)
            seen_idx[i] = false;
    }
}

/* The required sequence function */
seq_t a(idx_t n) {
    /* Allocate/resize seq array as needed (1‑based) */
    if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* Allocate/resize seen_idx array as needed (1‑based) */
    if (seen_cap <= (size_t)n) {
        enlarge_seen_idx((size_t)n + 1);
    }

    /* First call – initialise */
    if (computed == 0) {
        seq[1] = (seq_t)1;
        if (seen_cap < 2) enlarge_seen_idx(2);
        seen_idx[1]      = true;
        seen_idx[0]      = false;      /* unused, keeps 1‑based indices tidy */
        computed = 1;
    }

    /* Compute values up to requested n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t prev = seq[i - 1];

            /* decide the increment */
            bool i_has_appeared = seen_idx[(size_t)i];
            seq_t inc = i_has_appeared ? (seq_t)3 : (seq_t)4;

            seq_t val = prev + inc;
            seq[i] = val;

            /* Record that 'val' has now appeared as a term.
               If val fits into the index type (it will for n up to 10000
               and even for the arbitrary‑precision build it stays well
               below 2^64 in the range we test), we mark it. */
            if (val <= (seq_t)((unsigned long long)~0ULL)) {
                size_t v = (size_t)val;
                if (v >= seen_cap)
                    enlarge_seen_idx(v + 1);
                seen_idx[v] = true;
            }
        }
        computed = n;
    }

    return seq[n];
}

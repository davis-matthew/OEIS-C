/*
 * A135490  Number of tieless basketball games from the years 1967-present
 *          with n scoring events.
 * https://oeis.org/A135490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A135490 – number of tieless basketball games with n scoring events.
 *
 *  A game is a sequence of scores chosen from {±1, ±2, ±3}.  Starting at
 *  total score 0, the partial sums must never be 0 after the first event.
 *
 *  a(0) = 1  (empty game)
 *  For n ≥ 1:
 *      a(n) = Σ_{s≠0}  b_n(s)         (sum of all allowable states after n events)
 *  where b_k(s) is the number of admissible prefixes of length k ended
 *  with cumulative score s.  The transition is
 *      b_{k+1}(s + t) += b_k(s)   for t ∈ {±1, ±2, ±3},
 *  with the restriction that b_{k+1}(0) is set to 0.
 *
 *  The implementation uses an incremental dynamic‑programming approach
 *  that stores, for each computed n, the vector of counts b_n(s).  The
 *  size of the vector for step k is 6k+1, usable indices are 0 … 6k.
 *  It memoizes all results in static arrays so that any calling order
 *  works.  The algorithm runs in O(n²) time and uses O(n) additional
 *  memory.  It works for any seq_t and idx_t supplied by the build system.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* shifts corresponding to the allowed scores */
static const int SHIFTS[6] = {-3, -2, -1, 1, 2, 3};

/* static memoisation state */
static idx_t      max_n   = 0;          /* largest n computed so far   */
static seq_t     *a_vals  = NULL;       /* a_vals[n] = a(n)             */
static size_t     a_cap   = 0;          /* size of a_vals array         */
static seq_t     *curr    = NULL;       /* counts after current step    */
static seq_t     *next    = NULL;       /* temporary array for next     */

/* Expand the internal arrays to handle all steps up to n, and
 * compute the missing a(k) values for old_n < k ≤ n. */
static void ensure_capacity(idx_t n)
{
    if (n <= max_n)
        return;                 /* already computed                */

    idx_t old_max = max_n;      /* previous maximum step           */
    idx_t new_n   = n;
    size_t new_size = (size_t)(6 * new_n + 1);

    /* Allocate new DP tables */
    seq_t *new_curr = (seq_t *)calloc(new_size, sizeof(seq_t));
    seq_t *new_next = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_curr || !new_next)
        exit(1);                /* out of memory                    */

    /* Copy existing DP values to the new tables          */
    if (old_max > 0) {
        size_t old_size = (size_t)(6 * old_max + 1);
        size_t delta    = 3 * (new_n - old_max);      /* shift offset */
        for (idx_t i = 0; i < old_size; ++i)
            if (curr[i] != 0)
                new_curr[i + delta] = curr[i];
    } else {
        /* Initial state for step 0 */
        new_curr[3 * new_n] = 1;   /* only the zero sum state       */
    }

    free(curr);
    free(next);
    curr = new_curr;
    next = new_next;

    /* Resize the sequence cache for a(n)              */
    if (!a_vals) {
        a_vals = (seq_t *)calloc(new_n + 1, sizeof(seq_t));
        if (!a_vals) exit(1);
        a_cap  = new_n + 1;
        a_vals[0] = (seq_t)1;
    } else if (new_n + 1 > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_vals, (new_n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = a_cap; i < new_n + 1; ++i)
            tmp[i] = (seq_t)0;
        tmp[0] = (seq_t)1;
        a_vals = tmp;
        a_cap  = new_n + 1;
    }

    /* Compute a(k) for each new step k = old_max+1 … new_n   */
    for (idx_t step = old_max + 1; step <= new_n; ++step) {
        /* clear next array                                    */
        memset(next, 0, new_size * sizeof(seq_t));

        /* propagate transitions from all existing states      */
        for (idx_t i = 0; i < new_size; ++i) {
            seq_t val = curr[i];
            if (!val)
                continue;
            for (int k = 0; k < 6; ++k) {
                int shift = SHIFTS[k];
                idx_t ni;
                if (shift >= 0) {
                    ni = i + shift;
                    if (ni >= new_size)
                        continue;  /* would land outside the array   */
                } else {
                    size_t abs_shift = (size_t)(-shift);
                    if (i < abs_shift)
                        continue;  /* underflow, skip               */
                    ni = i - abs_shift;
                }
                next[ni] += val;
            }
        }

        /* forbid landing on total score 0 after this event    */
        idx_t zero_index = 3 * step;
        if (zero_index < new_size)
            next[zero_index] = (seq_t)0;

        /* sum all non‑zero states to obtain a(step)           */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < new_size; ++i)
            sum += next[i];
        a_vals[step] = sum;

        /* swap curr and next for the next iteration          */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
    }

    max_n = new_n;
}

/* Public API required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) is defined as 1              */

    ensure_capacity(n);
    return a_vals[n];
}

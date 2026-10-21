/*
 * A070791  Integers n such that the 'Reverse and Add!' trajectory of n joins
 *          the trajectory of 5.
 * https://oeis.org/A070791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th positive integer whose reverse‐and‐add
 *            trajectory eventually reaches the trajectory of 5. The function
 *            builds the set of numbers that join 5's trajectory by simulating
 *            reverse‑and‑add steps and storing all discovered elements in
 *            sorted order
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 19, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   in_array, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"   /* seq_t reverse_decimal(seq_t x); */
#include "../utils/in_array.h"

/* local storage for the sequence and the trajectory of 5 */
static seq_t *seq = NULL;          /* sequence itself (ascending)          */
static idx_t seq_len = 0, seq_cap = 0;

static seq_t *traj5 = NULL;       /* trajectory of 5 (ascending)         */
static idx_t traj5_len = 0, traj5_cap = 0;

/* next integer to be tested (less than the first yet unknown term) */
static idx_t next_candidate = 6;

/* grow an array to fit at least `needed` elements */
static void grow_array(seq_t **arr, idx_t *cap, idx_t needed)
{
    if (*cap < needed) {
        *cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (seq_t*)realloc(*arr, *cap * sizeof(seq_t));
    }
}

/* extend the trajectory of 5 to include all elements up to `threshold` */
static void ensure_traj5_atleast(seq_t threshold)
{
    while (traj5_len == 0 || traj5[traj5_len-1] < threshold) {
        seq_t last = (traj5_len == 0) ? (seq_t)5 : traj5[traj5_len-1];
        seq_t next = last + reverse_decimal(last);
        grow_array(&traj5, &traj5_cap, traj5_len + 1);
        traj5[traj5_len++] = next;
    }
}

/* does the value `value` eventually hit 5's trajectory? */
static bool joins_five(seq_t value)
{
    seq_t x = value;
    /* a small fixed bound is sufficient for all known terms. */
    for (idx_t step = 0; step < 200; ++step) {
        ensure_traj5_atleast(x);
        if (in_array(traj5, traj5_len, x) || in_array(seq, seq_len, x))
            return true;
        x = x + reverse_decimal(x);
    }
    return false;
}

/* build the sequence until we have at least `n` terms */
static void ensure_terms(idx_t n)
{
    if (seq_len >= n) return;

    /* one‑time initialisation */
    if (seq == NULL) {
        grow_array(&seq, &seq_cap, 8);
        seq[0] = (seq_t)5;
        seq_len = 1;

        grow_array(&traj5, &traj5_cap, 8);
        traj5[0] = (seq_t)5;
        traj5_len = 1;

        next_candidate = 6;
    }

    while (seq_len < n) {
        seq_t cand = (seq_t)next_candidate++;
        if (joins_five(cand)) {
            grow_array(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = cand;
        }
    }
}

/* required public function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq[n-1];
}

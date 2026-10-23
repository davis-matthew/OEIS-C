/*
 * A043603  Numbers whose base-5 representation has exactly 3 runs.
 * https://oeis.org/A043603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   runs_in_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑5 representation has exactly 3 runs (OEIS A043603) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/runs_in_base5.h"

/* External integer types supplied by the build */
extern seq_t a(idx_t n);

/* Static state for memoisation */
static seq_t *seq = NULL;          /* array of found numbers */
static size_t seq_len = 0;         /* number of stored elements */
static size_t seq_cap = 0;         /* allocated capacity of the array */
static seq_t last_checked = 0;     /* largest integer examined so far */

/* Grow the array capacity when needed */
static void ensure_capacity(size_t needed)
{
    if (seq_len + needed <= seq_cap)
        return;

    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    while (new_cap < seq_len + needed)
        new_cap *= 2;

    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate numbers until we have at least n elements in seq[] */
static void generate_up_to(idx_t n)
{
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < n) {
        seq_t candidate = last_checked + 1;

        /* Count runs of candidate's base‑5 digits */
        int runs = runs_in_base5(candidate);

        if (runs == 3) {
            ensure_capacity(1);
            seq[seq_len++] = candidate;
        }

        last_checked = candidate;
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for the sequence, but safe */

    /* Generate terms only as far as needed */
    if (seq_len < n)
        generate_up_to(n);

    return seq[n - 1];
}

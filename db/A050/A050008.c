/*
 * A050008  a(n) = floor(a(n-1)/2) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 6*a(n-1).
 * https://oeis.org/A050008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 244, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(a(n-1)/2) if this is positive and not already in
 * the sequence; otherwise a(n) = 6*a(n-1).
 */
#include <stdbool.h>
#include <stdlib.h>

static seq_t *seq = NULL;      /* 1‑based array: seq[1] = a(1) */
static size_t seq_len = 0;     /* last index that has been computed */
static size_t seq_cap = 0;     /* current allocated capacity */

/* Ensure the array can hold indices up to `needed` (1‑based). */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Compute the sequence up to at least index `n`. */
static void compute_up_to(idx_t n)
{
    grow_seq((size_t)n + 1);          /* indices 0 .. n */

    /* Initialisation: a(1) = 1.  seq[0] is unused. */
    if (seq_len == 0) {
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        seq_len = 1;
    }

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t prev  = seq[(size_t)i - 1];
        seq_t cand  = prev / (seq_t)2;
        bool use_cand = false;

        if (cand > (seq_t)0) {
            /* Linear search for an earlier occurrence of cand. */
            bool found = false;
            for (idx_t j = 1; j < i; ++j) {
                if (seq[(size_t)j] == cand) {
                    found = true;
                    break;
                }
            }
            if (!found)
                use_cand = true;
        }

        seq[(size_t)i] = use_cand ? cand : prev * (seq_t)6;
    }
    seq_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;            /* undefined for n < 1; return 0 as a safety */

    if ((size_t)n > seq_len)
        compute_up_to(n);

    return seq[(size_t)n];
}

/*
 * A036779  Greedy sequence such that no term is the average of three others.
 * https://oeis.org/A036779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy sequence such that no term is the average of three others. */
/* The algorithm builds the sequence incrementally: for each new term
   it picks the smallest non‑negative integer that does not create
   a triple (a, b, c) with (a+b+c)/3 equal to some existing element.
   The sequence is strictly increasing and is stored in a static
   array so that subsequent calls can reuse already computed values. */

#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the generated sequence */
static seq_t *seq = NULL;      /* array of sequence values          */
static size_t seq_cap = 0;     /* allocated capacity (in elements) */
static idx_t seq_len = 0;      /* number of values stored          */

/* Ensure the array has room for at least 'need' elements.  */
static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Test whether a candidate value can be appended without violating
   the "no average of three" property.  */
static bool candidate_ok(const seq_t *arr, idx_t m, seq_t cand)
{
    if (m < 2)            /* fewer than two earlier elements – no conflict */
        return true;

    /* Compute the minimum and maximum possible sums of two distinct
       earlier elements to allow an early skip of impossible targets. */
    seq_t min_pair = arr[0] + arr[1];
    seq_t max_pair = arr[m - 1] + arr[m - 2];

    for (idx_t i = 0; i < m; ++i) {
        seq_t t = arr[i];

        /* If 3*t < cand, the required sum would be negative – skip. */
        if ((seq_t)3 * t < cand)
            continue;

        seq_t target = ((seq_t)3 * t) - cand;
        if (target < min_pair || target > max_pair)
            continue;

        /* Two‑pointer search for a pair (a,b) with a+b == target. */
        idx_t left = 0, right = m - 1;
        while (left < right) {
            seq_t sum = arr[left] + arr[right];
            if (sum == target) {
                if (arr[left] != t && arr[right] != t)
                    return false;          /* violation found */
                /* If the pair involves t, keep looking. */
            }
            if (sum <= target)
                ++left;
            if (sum >= target)
                --right;
        }
    }
    return true;            /* candidate is safe */
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined; return 0 to avoid crash */

    /* Allocate initial buffer on first call. */
    if (!seq) {
        seq = (seq_t *)malloc(16 * sizeof(seq_t));
        seq_cap = 16;
        seq_len = 0;
    }

    /* Build sequence up to index n (1‑based). */
    while (seq_len < n) {
        seq_t cand = (seq_len == 0) ? 0 : seq[seq_len - 1] + 1;
        while (!candidate_ok(seq, seq_len, cand))
            ++cand;           /* skip to next candidate */

        ensure_capacity(seq_len + 1);
        seq[seq_len++] = cand;
    }

    return seq[n - 1];
}

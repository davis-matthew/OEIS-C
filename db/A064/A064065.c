/*
 * A064065  n-th step is to add a(n) to each previous number a(k) (excluding
 *          itself, i.e., k < n) to produce n more terms of the sequence,
 *          starting with a(0)=0, a(1)=1.
 * https://oeis.org/A064065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1; for i≥1, append a(k)+a(i) for k=0..i‑1 to produce i new terms. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for the sequence values. */
    static seq_t *seq = NULL;
    static size_t size = 0;      /* number of stored values */
    static size_t capacity = 0;  /* allocated size of seq */
    static idx_t step_idx = 1;   /* index of the term whose value will generate the next block */

    /* Initialise on first call. */
    if (seq == NULL) {
        capacity = 4;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (seq == NULL) abort();
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        size = 2;
    }

    /* Return early if the answer is already known. */
    if (n < 2)
        return seq[(size_t)n];

    /* Generate terms until we have a[n]. */
    while (size <= (size_t)n) {
        /* Ensure enough capacity for the upcoming block. */
        if (size + step_idx > capacity) {
            size_t new_cap = capacity * 2;
            if (new_cap < size + step_idx) new_cap = size + step_idx;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            seq = tmp;
            capacity = new_cap;
        }

        seq_t val = seq[step_idx];          /* current generating value */
        size_t k, base = size;              /* start index for new block */

        for (k = 0; k < (size_t)step_idx; ++k)
            seq[base + k] = seq[k] + val;

        size += step_idx;   /* block of step_idx terms added */
        step_idx++;         /* next generator will be a[step_idx] */
    }

    return seq[(size_t)n];
}

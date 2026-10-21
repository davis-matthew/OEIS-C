/*
 * A064067  n-th step is to add a(n) to each previous number a(k) (excluding
 *          itself, i.e., k < n) to produce n more terms of the sequence,
 *          starting with a(0)=1, a(1)=2.
 * https://oeis.org/A064067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2;
   For n ≥ 1, append to the sequence the values a(n)+a(k) for all k < n.
   The sequence generated is OEIS A064067. */

#include <stdlib.h>

static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap * 2);
    while (new_cap < needed) new_cap *= 2;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static seq_t *seq = NULL;
    static size_t len = 0;          /* number of elements stored */
    static size_t cap = 0;          /* allocated capacity */
    static idx_t next_step = 1;     /* next step index to be processed */

    if (seq == NULL) {              /* first call: initialise */
        grow_seq(&seq, &cap, 16);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        len = 2;
        /* next_step remains 1 so the first generated step will be n=1 */
    }

    size_t target = (size_t)n;
    while (len <= target) {
        seq_t val = seq[next_step];           /* a(next_step) is already stored */
        for (idx_t k = 0; k < next_step; ++k) {
            seq_t s = val + seq[k];
            grow_seq(&seq, &cap, len + 1);
            seq[len++] = s;
        }
        ++next_step;
    }

    return seq[target];
}

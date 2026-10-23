/*
 * A100143  Unique sequence with a(1)=1 where each a(n) occurs in the same
 *          order a(n) times consecutively in its sequence of first
 *          differences which contains no other terms.
 * https://oeis.org/A100143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A100143:  a(1) = 1.  For each block starting at index s (1‑based) with value v = a(s),
   add v new terms each incremented by v.  Thus the first differences contain
   each a(n) exactly a(n) times consecutively. */

#include <stdlib.h>

static void ensure_capacity(idx_t min_cap, seq_t **arr, idx_t *cap) {
    if (*cap < min_cap) {
        idx_t new_cap = (*cap) ? (*cap) << 1 : 16;
        while (new_cap < min_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Lazily build the sequence up to the required index.  The sequence is
       constructed in O(n) time, storing the terms in a dynamic array. */
    static seq_t *seq = NULL;
    static idx_t size = 0;      /* number of terms currently stored */
    static idx_t cap = 0;       /* current capacity of the array */
    static idx_t next_block = 1;/* index of the next block to process (1‑based) */

    /* Initialise on first call. */
    if (size == 0) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)1;     /* a(1) = 1 */
        size = 1;
        next_block = 1;
    }

    /* Generate further terms until a(n) is available. */
    while (size < n) {
        /* Process the block starting at index next_block. */
        seq_t v = seq[next_block - 1];           /* v = a(next_block) */
        idx_t cnt = (idx_t)v;                    /* number of repetitions (safe cast) */

        for (idx_t t = 1; t <= cnt; ++t) {
            ensure_capacity(size + 1, &seq, &cap);
            seq[size] = seq[size - 1] + v;        /* a(new) = a(prev) + v */
            ++size;
        }
        ++next_block;
    }

    return seq[n - 1];
}

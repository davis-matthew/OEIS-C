/*
 * A032816  Numbers whose set of base-16 digits is {2,3}.
 * https://oeis.org/A032816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth smallest positive integer whose hexadecimal
 *            representation consists solely of the digits 2 and 3
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 510, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence on demand using a breadth‑first traversal:
   Starting with the two one‑digit numbers 2 and 3, each stored element generates
   two children seq[i]*16 + 2 and seq[i]*16 + 3.
   The array is grown only as much as needed to satisfy the requested index. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    static idx_t gen_idx = 0;   /* next index whose children should be generated */

    /* Initialize first two sequence elements */
    if (seq_len == 0) {
        seq_cap = 4;                     /* initial capacity */
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq) return 0;              /* allocation failure (unlikely for tests) */
        seq[0] = 2;
        seq[1] = 3;
        seq_len = 2;
    }

    /* Grow the cache until we have at least n elements */
    while (seq_len < n) {
        /* Ensure enough space for two new elements */
        if (seq_len + 2 > seq_cap) {
            seq_cap = seq_cap * 2 + 4;   /* exponential growth */
            seq = (seq_t *)realloc(seq, sizeof(seq_t) * seq_cap);
            if (!seq) return 0;         /* allocation failure (unlikely) */
        }

        /* Generate children of seq[gen_idx] */
        seq_t parent = seq[gen_idx];
        seq[seq_len++] = parent * ((seq_t)16) + 2;
        seq[seq_len++] = parent * ((seq_t)16) + 3;
        gen_idx++;                      /* move to next parent */
    }

    /* Array is 0‑based; input n is 1‑based */
    return seq[n - 1];
}

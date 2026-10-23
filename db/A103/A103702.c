/*
 * A103702  Add 3 to each of the preceding digits, beginning with 1.
 * https://oeis.org/A103702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A103702:
   a(1) = 1.
   For n > 1 the terms are produced by a breadth‑first traversal of a tree
   rooted at 1.  A node x has children formed by adding 3 to each
   decimal digit of x (left‑to‑right order).  Each child is a new term of
   the sequence, and the order of visitation (queue) yields the terms
   listed in OEIS. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for computed terms and queue index */
    static seq_t *seq      = NULL;
    static idx_t seq_len   = 0;    /* number of terms stored */
    static idx_t proc_idx = 0;    /* next term to process as a node */
    static size_t cap      = 0;    /* capacity of seq array */

    if (n == 0) return 0;         /* offset is 1; safe guard */

    /* initialise with the root node if needed */
    if (seq_len == 0) {
        cap = 1;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t parent = seq[proc_idx];

        /* extract decimal digits of parent (most significant first) */
        unsigned int digits[64];
        int top = 0;
        seq_t tmp = parent;
        while (tmp > 0) {
            digits[top++] = (unsigned int)(tmp % 10);
            tmp /= 10;
        }
        if (top == 0) {              /* safety; shouldn't happen */
            digits[top++] = 0;
        }

        /* push children in the order of most‑significant to least */
        for (int i = top - 1; i >= 0; --i) {
            seq_t child = (seq_t)(digits[i] + 3);
            if (seq_len == cap) {     /* grow array when full */
                cap <<= 1;
                if (cap == 0) cap = 1;    /* safety */
                seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
            }
            seq[seq_len++] = child;
        }

        proc_idx++;   /* move to next node to process */
    }

    return seq[n - 1];
}

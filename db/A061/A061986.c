/*
 * A061986  Number of times n appears in sequence b(k) = 1 + b(floor(k/2)) +
 *          b(floor(k/3)) with b(0) = 0, i.e., in A061984.
 * https://oeis.org/A061986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061986 – number of times a given n appears in
 * the recursively defined sequence b:
 *   b(0) = 0
 *   b(k) = 1 + b(floor(k/2)) + b(floor(k/3))
 * The function generates b(k) incrementally until the first
 * value greater than n is reached, and counts how many times
 * n occurs.  */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static storage for the evolving list of b(k) values */
    static seq_t *b = NULL;
    static size_t cap = 0;   /* capacity of the array */
    static idx_t  len = 0;   /* number of elements stored */

    /* first‑time initialisation */
    if (b == NULL) {
        cap = 64;                          /* arbitrary starting size */
        b = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!b) return 0;                  /* fallback for OOM */
        b[0] = (seq_t)0;                   /* b(0) = 0 */
        len = 1;
    }

    /* extend the sequence until the last element exceeds n */
    while (b[len - 1] <= (seq_t)n) {
        idx_t k = len;                     /* new index */
        seq_t val = (seq_t)1 + b[k / 2] + b[k / 3];

        /* ensure enough space */
        if ((size_t)len >= cap) {
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(b, cap * sizeof(seq_t));
            if (!tmp) return 0;           /* fallback for OOM */
            b = tmp;
        }
        b[len++] = val;                    /* store new value */
    }

    /* count occurrences of n (sequence is non‑decreasing,
       so we can stop once values exceed n) */
    seq_t cnt = 0;
    for (idx_t i = 0; i < len && b[i] <= (seq_t)n; ++i) {
        if (b[i] == (seq_t)n) ++cnt;
    }
    return cnt;
}

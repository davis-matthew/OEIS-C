/*
 * A081037  Inverse binary transform of A027656.
 * https://oeis.org/A081037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-1) - 4*a(n-2) for n >= 4 Initial values: a(0)=1,
 *            a(1)=-1, a(2)=3, a(3)=-7
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 28, i64 n <= 59, i128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoized storage for computed terms */
    static seq_t *seq = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* First call: allocate and store the first four terms */
    if (!seq) {
        cap = 8;                     /* initial capacity */
        seq = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)(-1);
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)(-7);
        len   = 4;
    }

    /* If the desired index is already computed, return it */
    if (n < len) return seq[(size_t)n];

    /* Compute successive terms up to n, growing the array as needed */
    while (len <= n) {
        /* recurrence: a(n) = -4*a(n-1) - 4*a(n-2) */
        seq_t val = - (seq_t)4 * seq[len - 1] - (seq_t)4 * seq[len - 2];

        /* grow array if capacity exceeded */
        if (len == cap) {
            size_t newcap = cap * 2;
            seq_t *newptr = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
            if (!newptr) exit(EXIT_FAILURE);
            seq = newptr;
            cap = newcap;
        }
        seq[len++] = val;
    }

    return seq[(size_t)n];
}

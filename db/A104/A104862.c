/*
 * A104862  First differences of A014292.
 * https://oeis.org/A104862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4), with initial values a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 62, i64 n <= 121,
 *            i128 n <= 244, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *seq = NULL;
    static idx_t size  = 0;   /* number of elements stored      */
    static idx_t cap   = 0;   /* allocated capacity             */

    /* Initialise first 5 terms on first call */
    if (seq == NULL) {
        cap = 8;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = 0;
        seq[1] = 1;
        seq[2] = 1;
        seq[3] = 1;
        seq[4] = 1;
        size = 5;  /* indices 0..4 are filled */
    }

    /* Extend array until we have a(n) */
    while (size <= n) {
        /* Compute next term using the 4‑term recurrence */
        seq_t next = 0;
        next += seq[size - 1];   /* a(n-1) */
        next += seq[size - 1];   /* 2*a(n-1) */
        next -= seq[size - 2];   /* -a(n-2) */
        next -= seq[size - 4];   /* -a(n-4) */

        /* Ensure capacity */
        if (size >= cap) {
            cap *= 2;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }

        seq[size] = next;
        size++;
    }

    return seq[n];
}

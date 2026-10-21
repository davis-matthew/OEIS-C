/*
 * A089498  Duplicate of A046729.
 * https://oeis.org/A089498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=0; for n>0, a(n)=∑_{k=0}^{n-1} b(k)^2 where b(0)=2, b(1)=4
 *            and b(k)=2*b(k-1)+b(k-2) for k≥2 (i.e. b satisfies a linear
 *            recurrence)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for b-values and cumulative squares */
    static seq_t *B = NULL;            /* b(0), b(1), ... */
    static seq_t *Prefix = NULL;       /* Prefix[i] = sum_{k=0}^{i-1} b(k)^2, Prefix[0]=0 */
    static size_t B_len = 0;           /* number of b-values stored */
    static size_t B_cap = 0;           /* allocated capacity for B */
    static size_t Prefix_len = 0;      /* number of Prefix entries stored (always B_len+1) */
    static size_t Prefix_cap = 0;      /* allocated capacity for Prefix */

    /* trivial case */
    if (n == 0)
        return (seq_t)0;

    /* initialize tables if necessary */
    if (B_len == 0) {
        B_cap = 2;
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
        if (!B) return (seq_t)0;       /* allocation failure, return 0 as fallback */

        B[0] = (seq_t)2;
        B[1] = (seq_t)4;
        B_len = 2;
    }
    if (Prefix_len < 3) { /* we need Prefix[0], Prefix[1], Prefix[2] at least */
        Prefix_cap = 3;
        Prefix = (seq_t *)malloc(Prefix_cap * sizeof(seq_t));
        if (!Prefix) return (seq_t)0;

        Prefix[0] = (seq_t)0;
        Prefix[1] = B[0] * B[0];                  /* sum of b[0]^2 */
        Prefix[2] = Prefix[1] + B[1] * B[1];       /* sum of b[0]^2 + b[1]^2 */
        Prefix_len = 3;
    }

    /* grow tables until we have at least up to index n-1 for b and n for Prefix */
    while (B_len < n) {
        /* ensure capacity for new B */
        if (B_len == B_cap) {
            size_t new_cap = B_cap * 2;
            seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failure */
            B = tmp;
            B_cap = new_cap;
        }

        /* compute next b value using recurrence */
        seq_t two = (seq_t)2;
        seq_t new_b = two * B[B_len - 1] + B[B_len - 2];
        B[B_len] = new_b;
        ++B_len;

        /* ensure capacity for new Prefix entry */
        if (Prefix_len == Prefix_cap) {
            size_t new_cap = Prefix_cap * 2;
            seq_t *tmp = (seq_t *)realloc(Prefix, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            Prefix = tmp;
            Prefix_cap = new_cap;
        }

        /* append new cumulative square */
        Prefix[Prefix_len] = Prefix[Prefix_len - 1] + new_b * new_b;
        ++Prefix_len;
    }

    /* return the n-th term a(n) = Prefix[n] */
    return Prefix[n];
}

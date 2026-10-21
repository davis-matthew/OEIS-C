/*
 * A057870  Number of singular points on n-th order Chmutov surface.
 * https://oeis.org/A057870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057870: Number of singular points on n‑th order Chmutov surface.
   Recurrence (n > 13):
      a(n) =  2*a(n-2) + a(n-3) - a(n-4) - 2*a(n-5) + a(n-6)
             + a(n-7) - 2*a(n-8) - a(n-9) + a(n-10) + 2*a(n-11) - a(n-13)
   Initial values:
      a(1)=0, a(2)=1, a(3)=3, a(4)=14, a(5)=28, a(6)=57, a(7)=93,
      a(8)=154, a(9)=216, a(10)=321, a(11)=425, a(12)=576, a(13)=732 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values (1‑based indexing) */
    static seq_t *seq = NULL;
    static size_t cap = 0;          /* allocated size (including unused index 0) */
    static size_t sz  = 0;          /* highest n that has been computed (>=1) */

    /* First time: allocate and initialise the first 13 terms. */
    if (!seq) {
        cap = 14; /* indices 0 .. 13 */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        /* 0‑th element unused; initialise all required terms. */
        seq[0] = (seq_t)0;          /* not used */
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)1;
        seq[3] = (seq_t)3;
        seq[4] = (seq_t)14;
        seq[5] = (seq_t)28;
        seq[6] = (seq_t)57;
        seq[7] = (seq_t)93;
        seq[8] = (seq_t)154;
        seq[9] = (seq_t)216;
        seq[10] = (seq_t)321;
        seq[11] = (seq_t)425;
        seq[12] = (seq_t)576;
        seq[13] = (seq_t)732;
        sz = 13;
    }

    /* Grow the cache if the requested index exceeds current capacity. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;      /* need space for indices 0 .. n */
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* If the value is already computed, return it immediately. */
    if ((size_t)n <= sz) {
        return seq[(size_t)n];
    }

    /* Compute missing terms up to n (recurrence valid for n > 13). */
    for (size_t i = sz + 1; i <= (size_t)n; ++i) {
        /* Compute positive and negative contributions separately to avoid
           underflow on unsigned types. */
        seq_t pos = (seq_t)0;
        pos += seq[i-2];
        pos += seq[i-2];               /* 2*a(i-2) */
        pos += seq[i-3];
        pos += seq[i-6];
        pos += seq[i-7];
        pos += seq[i-10];
        pos += seq[i-11];
        pos += seq[i-11];              /* 2*a(i-11) */

        seq_t neg = (seq_t)0;
        neg += seq[i-4];
        neg += seq[i-5];
        neg += seq[i-5];               /* 2*a(i-5) */
        neg += seq[i-8];
        neg += seq[i-8];               /* 2*a(i-8) */
        neg += seq[i-9];
        neg += seq[i-13];

        seq[i] = pos - neg;
    }

    sz = (size_t)n;
    return seq[(size_t)n];
}

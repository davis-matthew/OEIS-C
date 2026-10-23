/*
 * A108413  Expansion of (1+x+5x^2+2x^3) / (1-4x^2+x^4).
 * https://oeis.org/A108413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=-2, for n>3: a(n) = -4*a(n-2) -
 *            a(n-4)
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 34, i64 n <= 67, i128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the function assumes the types seq_t and idx_t are supplied elsewhere
   (seq_t may be an arbitrary‑precision integer class in a C++ build). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation table */
    static seq_t *table   = NULL;
    static size_t  size   = 0;   /* number of values stored */
    static size_t  cap    = 0;   /* allocated capacity */

    /* initialise the first four terms on first use */
    if (size == 0) {
        cap   = 4;
        table = (seq_t*)malloc(cap * sizeof(seq_t));
        table[0] = (seq_t)1;
        table[1] = (seq_t)1;
        table[2] = (seq_t)1;
        table[3] = (seq_t)-2;
        size   = 4;
    }

    /* grow the table if necessary */
    if ((idx_t)size <= n) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            cap = needed;
            table = (seq_t*)realloc(table, cap * sizeof(seq_t));
        }

        /* compute missing values up to n */
        for (size_t i = size; i <= (size_t)n; ++i) {
            seq_t val;
            if (i <= 3) {
                /* first four terms already set */
                continue;
            } else {
                /* recurrence: a(i) = -4*a(i-2) - a(i-4) */
                seq_t t1 = table[i - 2];
                t1 = t1 * ((seq_t)4);
                val = -t1 - table[i - 4];
            }
            table[i] = val;
        }
        size = (size_t)n + 1;
    }

    return table[(size_t)n];
}

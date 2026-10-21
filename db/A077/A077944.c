/*
 * A077944  Expansion of 1/(1-2*x+2*x^2+x^3).
 * https://oeis.org/A077944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 38, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=2; for n>=3: a(n)=2*a(n-1)-2*a(n-2)-a(n-3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;

    if (n >= cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        /* Assume allocation succeeds */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0) seq[i] = 1;
            else if (i == 1) seq[i] = 2;
            else if (i == 2) seq[i] = 2;
            else seq[i] = (seq_t)2 * seq[i - 1]
                        - (seq_t)2 * seq[i - 2]
                        - seq[i - 3];
        }
        cap = new_cap;
    }
    return seq[(size_t)n];
}

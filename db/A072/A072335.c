/*
 * A072335  Expansion of 1/((1-x^2)*(1-4*x+x^2)).
 * https://oeis.org/A072335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-3) + a(n-4) (a(0)=1, a(1)=4, a(2)=16,
 *            a(3)=60)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static array of computed values */
    static seq_t *seq = NULL;
    static size_t  capacity = 0;   /* allocated array size   */
    static size_t  length   = 0;   /* number of terms computed */

    /* ensure we have enough space for indices up to n */
    if (capacity < (size_t)n + 1) {
        size_t newcap = (capacity > 0) ? capacity * 2 : 4;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
        if (!seq) abort();      /* allocation failed */
        capacity = newcap;
    }

    /* initialise base terms when needed */
    if (length == 0) {
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        seq[2] = (seq_t)16;
        seq[3] = (seq_t)60;
        length = 4;
    } else if (length < 4) {        /* fill any missing small terms */
        if (length == 1 && n >= 1) { seq[1] = (seq_t)4; length = 2; }
        if (length == 2 && n >= 2) { seq[2] = (seq_t)16; length = 3; }
        if (length == 3 && n >= 3) { seq[3] = (seq_t)60; length = 4; }
    }

    /* compute terms up to n */
    for (size_t i = length; i <= (size_t)n; ++i) {
        seq[i] = (seq_t)4 * seq[i-1] - (seq_t)4 * seq[i-3] + seq[i-4];
    }
    length = (size_t)n + 1;

    return seq[(size_t)n];
}

/*
 * A077952  Expansion of 1/(1 - x + x^2 + 2*x^3).
 * https://oeis.org/A077952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2) - 2*a(n-3), with a(0)=1, a(1)=1, a(2)=0
 * Range:     i8 n <= 12, i16 n <= 23, i32 n <= 48, i64 n <= 97, i128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *data = NULL;
    static size_t len = 0;          /* number of elements currently stored */

    size_t need = (size_t)n + 1;    /* size required to store a[0..n] */

    if (data == NULL) {
        data = (seq_t *)malloc(need * sizeof(seq_t));
    } else if (len < need) {
        data = (seq_t *)realloc(data, need * sizeof(seq_t));
    } else {
        /* already have enough space */
        return data[n];
    }

    /* Initialise base values if required */
    size_t start = len;             /* index from which to start computing */
    if (start < 3) {
        if (need > 0) data[0] = (seq_t)1;
        if (need > 1) data[1] = (seq_t)1;
        if (need > 2) data[2] = (seq_t)0;
        start = 3;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = start; i < need; ++i) {
        data[i] = data[i-1] - data[i-2] - ((seq_t)2 * data[i-3]);
    }

    len = need;
    return data[n];
}

/*
 * A186830  Keith sequence for the number 197.
 * https://oeis.org/A186830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3); a(1) = 1, a(2) = 9, a(3) = 7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 1 */
    if (n < 1)
        return (seq_t)0;   /* undefined for n = 0, but safe return */

    /* 0‑based array:  a(n) is stored in buf[n‑1] */
    static seq_t *buf = NULL;   /* dynamic cache of computed terms */
    static size_t cap   = 0;    /* allocated size of the array */
    static size_t max_n = 0;    /* number of values that have been computed */

    /* we need at least `n` cells (indices 0 … n‑1) */
    size_t required = (size_t)n;
    if (required > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0)        /* allocation failure – abort safely */
            return (seq_t)0;
        buf = tmp;
        cap = new_cap;
    }

    /* seed the first three terms if needed */
    if (max_n < 3) {
        buf[0] = (seq_t)1;                /* a(1) */
        if (cap > 1) buf[1] = (seq_t)9;    /* a(2) */
        if (cap > 2) buf[2] = (seq_t)7;    /* a(3) */
        max_n = 3;
    }

    /* compute any missing terms up to n */
    for (size_t i = max_n; i < required; ++i)
        buf[i] = buf[i-1] + buf[i-2] + buf[i-3];

    max_n = required;

    return buf[required - 1];   /* a(n) */
}

/*
 * A077955  Expansion of 1/(1-x+2*x^2+x^3).
 * https://oeis.org/A077955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 47, i64 n <= 94, i128 n <= 190,
 *            big any n
 * Validated: standard, pass, 58 terms through n = 885 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=-1;
   a(n)=a(n-1)-2*a(n-2)-a(n-3)  for n>=3
   (from 1/(1-x+2x^2+x^3)) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buf_size = 0;   /* number of elements stored (<= size of buf) */

    /* initialise first three terms if needed */
    if (buf == NULL) {
        buf_size = 3;
        buf = mem_new(seq_t, buf_size);
        if (!buf) return 0;              /* allocation failed, return 0 as fallback */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)-1;
    }

    /* return already computed value */
    if (n < buf_size)
        return buf[n];

    /* extend buffer to hold up to n */
    seq_t *tmp = mem_resize(seq_t, buf, (n + 1));
    if (!tmp) return 0;          /* realloc failed, return 0 as fallback */
    buf = tmp;

    /* compute from first missing index up to n */
    for (idx_t i = buf_size; i <= n; ++i) {
        buf[i] = buf[i-1] - (seq_t)2 * buf[i-2] - buf[i-3];
    }
    buf_size = n + 1;

    return buf[n];
}

/*
 * A104700  Duplicate of A039941.
 * https://oeis.org/A104700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=0, a(1)=1. For n >= 2: if n is even: a(n) = a(n-1) + a(n-2)
 *            if n is odd : a(n) = a(n-1) * a(n-2)
 * Range:     i8 n <= 10, i16 n <= 12, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation array */
    static seq_t *buf = NULL;
    static idx_t buf_cap = 0;
    static idx_t buf_last = 0;   /* largest index computed so far */

    /* initialise for indices 0 and 1 */
    if (buf == NULL) {
        buf_cap = 2;
        buf = (seq_t*)malloc(sizeof(seq_t) * buf_cap);
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)1;
        buf_last = 1;
    }

    /* if the requested index is already known, return it */
    if (n <= buf_last) {
        return buf[n];
    }

    /* grow the buffer if necessary */
    if (n >= buf_cap) {
        idx_t new_cap = buf_cap * 2;
        if (new_cap < n + 1) {
            new_cap = n + 1;
        }
        buf = (seq_t*)realloc(buf, sizeof(seq_t) * new_cap);
        buf_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = buf_last + 1; i <= n; ++i) {
        if ((i & 1) == 0) {          /* even index */
            buf[i] = buf[i - 1] + buf[i - 2];
        } else {                     /* odd index */
            buf[i] = buf[i - 1] * buf[i - 2];
        }
    }

    buf_last = n;
    return buf[n];
}

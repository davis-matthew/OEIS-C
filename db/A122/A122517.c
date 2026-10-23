/*
 * A122517  G.f.: 1/(1 - x^3 - 2 x^4 + x^5).
 * https://oeis.org/A122517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 45, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1, a(1) = 0, a(2) = 0
   a(n) = a(n-3) + 2*a(n-4) - a(n-5)   for n >= 3
   (terms with negative indices are treated as 0)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *data = NULL;               /* cached values */
    static idx_t capacity = 0;               /* allocated size */
    static idx_t size      = 0;              /* last computed index + 1 */

    /* helper to grow the cache so that indices up to n are usable */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        data = tmp;
        /* initialize the new slots with zero (they will be overwritten) */
        for (idx_t i = capacity; i < new_cap; ++i) data[i] = 0;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            data[0] = 1;
        } else if (i == 1 || i == 2) {
            data[i] = 0;
        } else {
            seq_t val = 0;
            if (i >= 3) val += data[i - 3];
            if (i >= 4) val += data[i - 4] * 2;
            if (i >= 5) val -= data[i - 5];
            data[i] = val;
        }
    }
    size = n + 1;      /* now indices 0..n are guaranteed computed */

    return data[n];
}

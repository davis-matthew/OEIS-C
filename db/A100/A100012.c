/*
 * A100012  Let h(k) = a(k)*((145*a(k)^3)-(280*a(k)^2)+(179*a(k))-38)/6, then
 *          a(n) = h(a(n-1)) for n >= 1 and a(0) =2.
 * https://oeis.org/A100012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   let h(k) = a(k)*((145*a(k)^3)-(280*a(k)^2)+(179*a(k))-38)/6,
 *            a(n) = h(a(n-1)), a(0) = 2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   compute_h
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compute_h.h"

/* Return the nth term of the sequence.  Uses memoisation so each term
   is only computed once, regardless of call order. */
seq_t
a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *values   = NULL;
    static idx_t size      = 0;
    static idx_t capacity  = 0;

    /* initialise on first call */
    if (!values) {
        capacity = 16;
        values   = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!values) return (seq_t)0;           /* allocation failure */
        values[0] = (seq_t)2;                   /* a(0) = 2 */
        size     = 1;
    }

    /* extend computed range to include a(n) */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            /* grow array if needed */
            if (i >= capacity) {
                capacity = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(values, capacity * sizeof(seq_t));
                if (!tmp) return (seq_t)0;     /* allocation failure */
                values = tmp;
            }
            values[i] = compute_h(values[i - 1]);
        }
        size = n + 1;
    }

    return values[n];
}

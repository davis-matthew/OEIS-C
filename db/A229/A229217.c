/*
 * A229217  If 1 and 2 represent the 2D vectors (1,0) and (0,1) and -1 and -2
 *          are the negation of these vectors, then this sequence represents
 *          the Koch curve.
 * https://oeis.org/A229217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Helpers:   koch_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.  For n > 1 the sequence is obtained by
   recursively applying the 5‑letter rewriting rules
   P(1)  = 1,2,1,-2,1
   P(2)  = 2,-1,2,1,2
   P(-1) = -1,-2,-1,2,-1
   P(-2) = -2,1,-2,-1,-2
   The argument n is 1‑based. */
#include <stddef.h>
#include "../utils/koch_symbol.h"

seq_t a(idx_t n)
{
    return koch_symbol(n);
}

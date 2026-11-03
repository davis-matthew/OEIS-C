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
 * Validated: partial, pass, 107 terms through n = 107 (2026-11-01)
 * Helpers:   none
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

static seq_t koch_symbol(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* previous index in the previous step (ceil division by 5) */
    idx_t pidx = (n - 1) / 5 + 1;
    seq_t prev = koch_symbol(pidx);

    /* position within the 5‑symbol expansion (0..4) */
    idx_t pos = (n - 1) % 5;
    seq_t res;

    if (prev == (seq_t)1) {
        switch (pos) {
            case 0: res =  1; break;
            case 1: res =  2; break;
            case 2: res =  1; break;
            case 3: res = -2; break;
            case 4: res =  1; break;
        }
    } else if (prev == (seq_t)-1) {
        switch (pos) {
            case 0: res = -1; break;
            case 1: res = -2; break;
            case 2: res = -1; break;
            case 3: res =  2; break;
            case 4: res = -1; break;
        }
    } else if (prev == (seq_t)2) {
        switch (pos) {
            case 0: res =  2; break;
            case 1: res = -1; break;
            case 2: res =  2; break;
            case 3: res =  1; break;
            case 4: res =  2; break;
        }
    } else { /* prev == -2 */
        switch (pos) {
            case 0: res = -2; break;
            case 1: res =  1; break;
            case 2: res = -2; break;
            case 3: res = -1; break;
            case 4: res = -2; break;
        }
    }

    return res;
}

seq_t a(idx_t n)
{
    return koch_symbol(n);
}

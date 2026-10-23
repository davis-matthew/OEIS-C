/*
 * A100727  Continued fraction expansion of (1/2) [tan(1) + sec(1)].
 * https://oeis.org/A100727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100727 – continued fraction partial quotients of (1/2)(tan 1 + sec 1).
   For n < 13 the terms are hard‑coded.
   For n ≥ 13 the terms follow the 4‑term cycle:
       k, 1, 2, 1
   where k increases by 1 each cycle starting at 3.
   Formally:
     if n < 13: a(n) = init[n]
     else:
         d = n – 13
         r = d % 4
         b = d / 4
         a(n) = (r == 0) ? (seq_t)(3 + (seq_t)b)
                 : (r == 1) ? (seq_t)1
                 : (r == 2) ? (seq_t)2
                 : (seq_t)1;
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 13 initial terms of the sequence */
    static const seq_t init[13] = {
        1, 1, 2, 2, 1, 1, 1, 2, 1, 2, 1, 2, 1
    };

    if (n < 13) {
        return init[(size_t)n];
    }

    idx_t d = n - 13;
    idx_t r = d % 4;
    idx_t b = d / 4;

    if (r == 0) {
        return (seq_t)(seq_t)3 + (seq_t)b;
    } else if (r == 1) {
        return (seq_t)1;
    } else if (r == 2) {
        return (seq_t)2;
    } else {
        return (seq_t)1;
    }
}

/*
 * A138614  Expansion of (2*x-1)*(x^2-x-1) / ( 1-2*x^2+2*x^4 ).
 * https://oeis.org/A138614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -4 * a(n-8) for n >= 8
   with initial values:
   a(0) = 1,  a(1) = -1,  a(2) = -1,  a(3) = 0,
   a(4) = -4, a(5) = 2,   a(6) = -6,  a(7) = 4 */

/* Using the relation a(n) = (-4)^(⌊n/8⌋) * a(n mod 8) */

#include "../utils/seq_pow.h"

static const seq_t init[8] = {
    (seq_t) 1,
    (seq_t)-1,
    (seq_t)-1,
    (seq_t) 0,
    (seq_t)-4,
    (seq_t) 2,
    (seq_t)-6,
    (seq_t) 4
};

seq_t a(idx_t n) {
    idx_t r = n & 7;          /* n mod 8 */
    idx_t k = n >> 3;         /* floor(n / 8) */

    if (k == 0)
        return init[r];
    if (init[r] == (seq_t)0)
        return (seq_t)0;

    seq_t pow = seq_pow((seq_t)-4, k);   /* (-4)^k */
    return pow * init[r];
}

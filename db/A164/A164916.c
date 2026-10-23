/*
 * A164916  Denominators of a BBP series for Pi/4.
 * https://oeis.org/A164916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 11, i32 n <= 25, i64 n <= 56, i128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 
 *   if n % 4 == 0:  (8*(n/4) + 1) * 16^(n/4)
 *   if n % 4 == 1: -2 * (8*(n/4) + 4) * 16^(n/4)
 *   if n % 4 == 2: -4 * (8*(n/4) + 5) * 16^(n/4)
 *   if n % 4 == 3: -4 * (8*(n/4) + 6) * 16^(n/4)
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute k = floor(n/4) and the remainder r */
    idx_t k = n / 4;
    unsigned r = n % 4;

    /* 16^k == 2^(4k) */
    seq_t pow16 = pow2(4 * k);

    /* base factor depends on the remainder */
    seq_t base;
    switch (r) {
        case 0:
            base = (seq_t)(8 * k + 1);
            break;
        case 1:
            base = - (seq_t)(2 * (8 * k + 4));
            break;
        case 2:
            base = - (seq_t)(4 * (8 * k + 5));
            break;
        case 3:
            base = - (seq_t)(4 * (8 * k + 6));
            break;
        default:
            /* unreachable, but satisfy compiler */
            base = 0;
            break;
    }

    return base * pow16;
}

/*
 * A091018  Numbers in n-th upward diagonal of triangle T : 0; 1, 2; 3, 4, 5;
 *          6, 7, 8, 9; ...
 * https://oeis.org/A091018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = element in the n-th upward diagonal of triangle T where
 *            triangle T contains 0; 1,2; 3,4,5; 6,7,8,9;
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   cum_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence enumerates the natural numbers by diagonals defined
   by r + c = d, listing each diagonal from largest r to smallest. */

#include <stdint.h>
#include <math.h>
#include "../utils/cum_terms.h"

static inline idx_t int_sqrt(idx_t x)
{
    /* floor(sqrt(x)) using double precision which is sufficient for
       idx_t up to 10^18 (double int precision up to 2^53). */
    return (idx_t)floor(sqrt((long double)x));
}

seq_t a(idx_t n)
{
    /* Find diagonal d that contains index n.  Diagonal d has
       length floor(d/2)+1 and cumulative count up to d is
       S(d) = (m+1)^2 when d=2m, or (m+1)(m+2) when d=2m+1. */
    idx_t m = int_sqrt(n);
    idx_t d;
    if (n < m * (m + 1))
        d = 2 * m - 1;   /* odd diagonal */
    else
        d = 2 * m;       /* even diagonal */

    /* Position within diagonal: offset = n - S(d-1) */
    idx_t prev_diag = (d == 0) ? (idx_t)-1 : d - 1;
    idx_t prev_cum = cum_terms(prev_diag);
    idx_t offset = n - prev_cum;   /* 0-based offset in this diagonal */

    /* In diagonal d, rows range from d down to ceil(d/2).
       offset corresponds to row r = d - offset, column c = offset. */
    idx_t r = d - offset;
    idx_t c = offset;

    /* Value = T_r + c, where T_r = r*(r+1)/2 */
    seq_t val = (seq_t)r * ((seq_t)r + 1) / 2 + (seq_t)c;
    return val;
}

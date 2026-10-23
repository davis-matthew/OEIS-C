/*
 * A190893  a(n) = [3en] - 3[en], where [ ] = floor.
 * https://oeis.org/A190893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*e*n) - 3*floor(e*n) (OEIS A190893)
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Computes a(n) for n >= 1.  Uses long double for sufficient precision
   and performs the arithmetic with integral types before converting to
   seq_t.  The result is always 0, 1, or 2. */
seq_t a(idx_t n)
{
    /* Calculate e*n and 3*e*n using long double */
    long double en       = (long double)n * expl(1.0L);   /* e*n            */
    long double three_en = 3.0L * en;                     /* 3*e*n          */

    /* Floor values cast to idx_t (uint64_t) */
    idx_t floor_en       = (idx_t)floorl(en);
    idx_t floor_three_en = (idx_t)floorl(three_en);

    /* Difference gives the desired sequence value (0 ≤ a(n) ≤ 2) */
    idx_t result = floor_three_en - 3 * floor_en;

    return (seq_t)result;
}

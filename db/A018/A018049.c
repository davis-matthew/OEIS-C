/*
 * A018049  Powers of fourth root of 2 rounded to nearest integer.
 * https://oeis.org/A018049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 29, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A018049: Powers of the fourth root of 2 rounded to nearest integer.
   Formula: a(n) = round(2^(n/4))   for n ≥ 0
   The function uses only integer arithmetic and the helper routines
   pow_int and floor_sqrt provided in the build. */

#include "../utils/pow_int.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a big integer. */
    seq_t two_n   = pow_int((seq_t)2, n);

    /* Integer fourth root floor: floor(2^(n/4)) = floor_sqrt( floor_sqrt(2^n) ). */
    seq_t sqrt1   = floor_sqrt(two_n);
    seq_t root    = floor_sqrt(sqrt1);   /* this is the lower bound of 2^(n/4) */

    /* Decide whether the fractional part of 2^(n/4) is >= 0.5.
       The inequality 2^(n/4) >= root + 0.5 is equivalent to
           2^(n+4) >= (2*root + 1)^4
       where the left side is 2^(n+4) and the right side is an integer. */
    seq_t left    = pow_int((seq_t)2, n + 4);         /* 2^(n+4) */
    seq_t u       = root * (seq_t)2 + (seq_t)1;      /* 2*root + 1 */
    seq_t right   = pow_int(u, (idx_t)4);             /* (2*root + 1)^4 */

    if (left >= right)
        return root + (seq_t)1;   /* round up */
    else
        return root;              /* round down */
}

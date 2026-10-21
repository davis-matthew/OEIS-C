/*
 * A200375  Product of Catalan and Jacobsthal numbers: a(n) =
 *          A000108(n)*A001045(n+1).
 * https://oeis.org/A200375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) * (2^(n+1)+(-1)^n) / 3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses catalan() from ../utils/catalan.h and pow2() from ../utils/pow2.h */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);          /* Catalan(n) */
    seq_t pow = pow2(n + 1);         /* 2^(n+1) */

    seq_t sum;
    if ((n & 1) == 0)                /* n even: +1 */
        sum = pow + 1;
    else                             /* n odd: -1 */
        sum = pow - 1;

    seq_t j = sum / 3;               /* Jacobsthal(n+1) */

    return cat * j;                  /* product */
}

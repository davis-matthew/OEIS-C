/*
 * A153973  a(n) = 3*a(n-1) - 2*a(n-2), with a(1) = 9, a(2) = 12.
 * https://oeis.org/A153973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3/2)*(4 + 2^n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(n) = 6 + 3*2^(n-1)  */
/*  With initial values a(1) = 9, a(2) = 12 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the base cases explicitly */
    if (n == 1U) return (seq_t)9;
    if (n == 2U) return (seq_t)12;

    /* For n >= 3 use the closed‑form expression:
       a(n) = 6 + 3 * 2^(n-1)  */
    seq_t two_pow = pow2(n - 1);    /* 2^(n-1) as seq_t */
    seq_t res = (seq_t)6 + ((seq_t)3) * two_pow;
    return res;
}

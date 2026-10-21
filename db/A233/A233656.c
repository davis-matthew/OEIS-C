/*
 * A233656  a(n) = 3*a(n-1) - 2*(n-1), with a(0) = 1.
 * https://oeis.org/A233656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 3^n + 2*n)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the closed‑form: (1 + 3^n + 2*n)/2 */
    seq_t pow3  = big_pow((seq_t)3, (seq_t)n);   /* 3^n */
    seq_t two_n = (seq_t)2 * (seq_t)n;           /* 2·n   */
    seq_t sum   = pow3 + (seq_t)1 + two_n;       /* 3^n + 1 + 2n */
    return sum / (seq_t)2;                       /* divide by 2 */
}

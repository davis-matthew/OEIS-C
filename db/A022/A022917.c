/*
 * A022917  Multinomial coefficient n!/ ([n/4]!, [(n+1)/4]!, [(n+2)/4]!,
 *          [(n+3)/4]!).
 * https://oeis.org/A022917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n! / (floor(n/4)! * floor((n+1)/4)! * floor((n+2)/4)! *
 *            floor((n+3)/4)!)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Split the four parts of the multinomial denominator. */
    idx_t a0 = n / 4;            /* floor(n/4) */
    idx_t b0 = (n + 1) / 4;      /* floor((n+1)/4) */
    idx_t c0 = (n + 2) / 4;      /* floor((n+2)/4) */
    /* d0 = (n + 3)/4;  unused explicitly, it equals n - (a0 + b0 + c0) */

    /* Compute the multinomial coefficient as a product of binomials. */
    seq_t res = binom(n, a0);                               /* C(n, a0) */
    res = seq_mul(res, binom(n - a0, b0));                  /* * C(n-a0, b0) */
    res = seq_mul(res, binom(n - a0 - b0, c0));             /* * C(n-a0-b0, c0) */

    return res;
}

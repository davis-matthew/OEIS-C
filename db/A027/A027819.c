/*
 * A027819  a(n) = 7*(n+1)*binomial(n+6,7)/2.
 * https://oeis.org/A027819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+1)*binomial(n+6,7)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*  Offset 1: a(1) corresponds to n = 1 */
    seq_t bin = binomial(n + 6, 7);   /* C(n+6,7) */
    seq_t res = bin * (seq_t)7;       /* multiply by 7 */
    res *= (seq_t)(n + 1);            /* multiply by (n+1) */
    res /= 2;                         /* divide by 2 */
    return res;
}

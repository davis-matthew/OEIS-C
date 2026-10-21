/*
 * A088852  Number of n X n matrices over GF(4) with characteristic polynomial
 *          x^(n-1) * (x-1).
 * https://oeis.org/A088852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4/3)*(4^(n^2-n) - 4^(n^2-2n)) for n ≥ 1, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    /* exponents for the two powers of 4 */
    seq_t e1 = (seq_t)n * ((seq_t)n - (seq_t)1); /* n^2 - n */
    seq_t e2 = (seq_t)n * ((seq_t)n - (seq_t)2); /* n^2 - 2n */

    seq_t pow1 = big_pow((seq_t)4, e1);   /* 4^(n^2-n)   */
    seq_t pow2 = big_pow((seq_t)4, e2);   /* 4^(n^2-2n)  */

    seq_t diff = pow1 - pow2;             /* divisible by 3   */
    seq_t temp = diff / (seq_t)3;         /* (4^(n^2-n)-4^(n^2-2n))/3 */
    seq_t result = (seq_t)4 * temp;       /* multiply by 4     */

    return result;
}

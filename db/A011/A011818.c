/*
 * A011818  Normalized volume of center slice of n-dimensional cube: 2^n*
 *          n!*Vol({ (x_1,...,x_n) in [ 0,1 ]^n: n/2 <= Sum_{i = 1..n} x_i <=
 *          (n+1)/2 }).
 * https://oeis.org/A011818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2 * Σ_{k=0..⌊(n+1)/2⌋} (-1)^k * binomial(n+1,k) *
 *            (n+1-2k)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* seq_t a(idx_t n) computes the OEIS sequence A011818 */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 the value is not used. */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;                     /* running signed sum of terms   */
    idx_t half = (n + 1) / 2;           /* floor((n+1)/2)                */

    for (idx_t k = 0; k <= half; ++k)
    {
        /* Core term: (-1)^k * binomial(n+1,k) * (n+1-2k)^n */
        seq_t base  = (seq_t)(n + 1 - 2 * k);
        seq_t power = seq_pow(base, n);            /* (n+1-2k)^n */
        seq_t bin   = binomial(n + 1, k);          /* binom(n+1,k) */
        seq_t term  = bin * power;                 /* unsigned product */

        if (k & 1)                                 /* apply sign */
            term = -term;                          /* assumes seq_t supports signed
                                                       subtraction/negative values */

        sum += term;                                 /* accumulating signed sum */
    }

    /* The result is guaranteed to be even. */
    return sum / 2;
}

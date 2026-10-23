/*
 * A035783  Coordination sequence for C_46 lattice.
 * https://oeis.org/A035783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^46
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Σ_{i=0..min(2n,46)} binom(46,i) * binom(45+2n-i,45)  */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    const idx_t K = 46;          /* exponent in the generating function */
    seq_t sum = 0;                /* accumulator for the coefficient */

    for (idx_t i = 0; i <= K; ++i)
    {
        /* If i > 2n the binomial term vanishes (0). */
        if (i > 2 * n)
            break;

        seq_t bin1 = binom(K, i);                 /* binom(46,i) */
        idx_t m    = 2 * n - i;                   /* exponent for the second binomial */
        seq_t bin2 = binom(45 + m, 45);           /* binom(45+m,45) == binom(45+m,m) */

        sum += bin1 * bin2;
    }

    return sum;
}

/*
 * A106050  Column two-from-center of triangle A059317.
 * https://oeis.org/A106050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d=0}^{⌊(n-3)/2⌋} binom(n-1-d, d) *
 *            sum_{b=0}^{⌊(n-3-2d)/2⌋} binom(n-1-2d, b+2) * binom(n-3-2d-b, b)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef INCLUDED_A106050
#define INCLUDED_A106050

/* Required helper: binom_small(idx_t N, idx_t K) is provided by the build. */
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* The sequence is 0 for n = 0, 1, 2. */
    if (n < 3)
        return 0;

    seq_t total = 0;
    /* maximum d value: floor((n-3)/2) */
    idx_t max_d = (n - 3) / 2;

    for (idx_t d = 0; d <= max_d; ++d)
    {
        /* binomial coefficient binom(n-1-d, d) */
        seq_t binom_nd = binom_small(n - 1 - d, d);

        /* parameters for inner sum */
        idx_t M     = n - 3 - 2 * d;        /* n-3-2d  */
        idx_t max_b = M / 2;                /* floor(M/2) */
        seq_t N1    = (seq_t)(n - 1 - 2 * d);   /* n-1-2d */
        seq_t N2    = (seq_t)(n - 3 - 2 * d);   /* n-3-2d */

        /* T(0) = binom(N1, 2) */
        seq_t T = N1 * (N1 - 1) / 2;
        seq_t inner = T;

        for (idx_t b = 0; b < max_b; ++b)
        {
            /* Compute R(b) = (N1-b-2)/(b+3) * (N2-2b)*(N2-2b-1)/((N2-b)*(b+1)) */
            seq_t num1 = N1 - (b + 2);          /* N1 - (b+2)   */
            seq_t den1 = (b + 3);               /* b+3          */
            seq_t num2 = (N2 - 2 * b) * (N2 - 2 * b - 1); /* (N2-2b)*(N2-2b-1) */
            seq_t den2 = (N2 - b) * (b + 1);                 /* (N2-b)*(b+1)   */

            seq_t num = num1 * num2;
            seq_t den = den1 * den2;

            T = T * num / den;
            inner += T;
        }

        total += binom_nd * inner;
    }

    return total;
}

#endif /* INCLUDED_A106050 */

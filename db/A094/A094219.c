/*
 * A094219  Number of permutations of length n with exactly 3 occurrences of
 *          the pattern 2-13.
 * https://oeis.org/A094219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * binomial(n+2,2) * binomial(2*n, n-5)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t c1 = binom(n + 2, 2);      /* binomial(n+2,2) = (n+2)(n+1)/2 */
    seq_t c2 = binom(2 * n, n - 5);  /* binomial(2*n,n-5) */

    /* Divide by 3 in the factor that is divisible by 3 to avoid overflow */
    seq_t res;
    if ((c1 % 3) == 0)
    {
        res = (c1 / 3) * c2;
    }
    else
    {
        res = c1 * (c2 / 3);
    }
    return res;
}

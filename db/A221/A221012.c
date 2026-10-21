/*
 * A221012  The Wiener index of the nanostar dendrimer G_n , defined
 *          pictorially in the Golriz - Darafsheh - Khalifeh reference.
 * https://oeis.org/A221012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3328 - 2^n*(1820*n-18616) + 4^n*(13520*n-14040)
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 22, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for int64_t */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1;             /* shift left multiplies by 2 */

    /* Compute 4^n = (2^n)^2 */
    seq_t pow4 = pow2 * pow2;

    /* Signed integer factors that may be negative */
    int64_t fac2 = 18616 - 1820 * (int64_t)n;          /* 18616 - 1820*n  */
    int64_t fac3 = 13520 * (int64_t)n - 14040;          /* 13520*n - 14040 */

    /* Accumulate the result */
    seq_t result = 0;

    if (fac2 >= 0)
        result += pow2 * (seq_t)fac2;
    else
        result -= pow2 * (seq_t)(-fac2);

    if (fac3 >= 0)
        result += pow4 * (seq_t)fac3;
    else
        result -= pow4 * (seq_t)(-fac3);

    /* Add the constant term (-3328) */
    result -= 3328;

    return result;
}

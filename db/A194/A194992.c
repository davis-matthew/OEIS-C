/*
 * A194992  Number of lower triangles of an n X n 0..2 array with each element
 *          differing from all of its horizontal and vertical neighbors by
 *          one.
 * https://oeis.org/A194992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for m>=1 a(2m) = 2^m * a(2m-1) a(2m+1) = (2^m *
 *            (2^(m+1)+1))/(2^m+1) * a(2m) (the fractional factor is always an
 *            integer)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Computes the n-th term of OEIS sequence A194992. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; omitted values may be returned unchanged */
        return (seq_t)1;   /* placeholder; not used for n>=1 */

    /* start from a(1) */
    seq_t result = (seq_t)3;

    for (idx_t i = 2; i <= n; ++i)
    {
        if ((i & 1) == 0)                    /* even index: i = 2*m */
        {
            idx_t m = i >> 1;
            result = result * pow2(m);
        }
        else                                 /* odd index: i = 2*m+1 */
        {
            idx_t m = (i - 1) >> 1;            /* m = (i-1)/2 */

            seq_t two_m      = pow2(m);
            seq_t two_mplus1 = pow2(m + 1);

            /* numerator = 2^m * (2^(m+1)+1) */
            seq_t numerator   = two_m * (two_mplus1 + (seq_t)1);
            /* denominator = 2^m + 1 */
            seq_t denominator = two_m + (seq_t)1;

            result = result * numerator / denominator;
        }
    }

    return result;
}

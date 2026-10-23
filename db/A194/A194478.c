/*
 * A194478  Number of ways to arrange 6 indistinguishable points on an n X n X
 *          n triangular grid so that no three points are in the same row or
 *          diagonal.
 * https://oeis.org/A194478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/256)*(-1)^n*(2*n - 7)*(n^2 - 7*n + 13) +
 *            (1/322560)*(7*n^12 + 42*n^11 - 945*n^10 + 1274*n^9 + 26089*n^8 -
 *            128810*n^7 + 175693*n^6 + 205366*n^5 - 810796*n^4 + 601328*n^3 +
 *            354172*n^2 - 582180*n + 114660)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 15, u64 n <= 98, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first term, but a(0) is unused). */
    if (n == 0) return (seq_t)0;

    /* Use 128‑bit arithmetic for intermediate values to avoid overflow
       while still being fast for all n where the result fits in the
       output type. */
    __int128 nn = (int64_t)n;

    /* Evaluate the 12th‑degree polynomial via Horner's method. */
    const int64_t coeff[13] = {7, 42, -945, 1274, 26089, -128810,
                               175693, 205366, -810796, 601328,
                               354172, -582180, 114660};
    __int128 val = coeff[0];
    for (int i = 1; i < 13; ++i) {
        val = val * nn + coeff[i];
    }

    /* Compute the first part: T1 = (-1)^n * (2*n - 7)*(n^2 - 7*n + 13) */
    __int128 t1 = (__int128)(2 * (int64_t)n - 7)
                * ((__int128)n * nn - 7 * nn + 13);
    __int128 part1_signed = ( (n & 1) ? (-t1) : t1 ) * 1260;  /* multiply by 1260 */

    /* Combine both parts: numerator = T2 + part1_signed
       (with a common denominator 322560). */
    __int128 numerator = val + part1_signed;

    /* Integer division gives the correct result (guaranteed divisible). */
    __int128 result = numerator / 322560;

    return (seq_t)result;
}

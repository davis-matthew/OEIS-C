/*
 * A191092  Number of n-cell polycubes that are proper in n-3 dimensions.
 * https://oeis.org/A191092
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-6)*n^(n-7)*(n-3)*(12*n^5 - 104*n^4 + 360*n^3 -
 *            679*n^2 + 1122*n - 1560)/3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(3)=0, a(4)=1 are special cases */
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)1;

    /* Use signed 128‑bit arithmetic for the polynomial so that
       intermediate signs are handled correctly.  For n≥5 the result is positive. */
    __int128 n128 = (int64_t)n;
    __int128 n2   = n128 * n128;
    __int128 n3   = n2 * n128;
    __int128 n4   = n3 * n128;
    __int128 n5   = n4 * n128;

    __int128 poly128 =
        12 * n5
      - 104 * n4
      + 360 * n3
      - 679 * n2
      + 1122 * n128
      - 1560;
    seq_t poly = (seq_t)poly128;

    seq_t num = (seq_t)(n - 3) * poly;

    /* Handle the 2‑power factor.  For n < 6 we divide by the corresponding
       positive power, otherwise we multiply. */
    if (n >= 6)
    {
        num *= big_pow((seq_t)2, (seq_t)(n - 6));
    }
    else
    {
        num /= big_pow((seq_t)2, (seq_t)(6 - n));
    }

    /* Handle the n‑power factor.  For n < 7 we divide by the corresponding
       positive power, otherwise we multiply. */
    if (n >= 7)
    {
        num *= big_pow((seq_t)n, (seq_t)(n - 7));
    }
    else
    {
        num /= big_pow((seq_t)n, (seq_t)(7 - n));
    }

    /* Final division by 3, which is guaranteed to be exact. */
    num /= (seq_t)3;

    return num;
}

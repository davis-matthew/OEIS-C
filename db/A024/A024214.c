/*
 * A024214  a(n) = 4th elementary symmetric function of first n+3 positive
 *          integers congruent to 1 mod 3.
 * https://oeis.org/A024214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(405*n^4+3510*n^3+9855*n^2+8814*n-184)/1920
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 309, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1; n=0 unused */
        return 0;

    unsigned __int128 nn   = n;
    unsigned __int128 prod = nn * (nn + 1) * (nn + 2) * (nn + 3);

    unsigned __int128 poly = (unsigned __int128)405 * nn * nn * nn * nn
                          + (unsigned __int128)3510 * nn * nn * nn
                          + (unsigned __int128)9855 * nn * nn
                          + (unsigned __int128)8814 * nn
                          - (unsigned __int128)184;

    unsigned __int128 numer  = prod * poly;
    unsigned __int128 result = numer / (unsigned __int128)1920;

    return (seq_t)result;
}

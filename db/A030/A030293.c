/*
 * A030293  Cubes with at most two different digits.
 * https://oeis.org/A030293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the cube of a number having at most two decimal digits.
 *            * The sequence can be described piecewise: * a(1…6) =
 *            {0,1,8,27,64,343} * a(7) = 10^3 = 1000 * a(8) = 11^3 = 1331 *
 *            a(9) = 8*10^3 = 8000 * For n ≥ 10: * let k = 2 + (n-10)/2 * if n
 *            is even: a(n) = 10^(3k) * if n is odd : a(n) = 8 * 1...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    static const seq_t smallCubes[6] = {0, 1, 8, 27, 64, 343};

    if (n <= 6)
        return smallCubes[n-1];

    if (n == 7)          /* 10^3 = 1000 */
        return pow10(3);

    if (n == 8)          /* 11^3 = 1331 */
        return 1331;

    if (n == 9)          /* 8*10^3 = 8000 */
        return (seq_t)8 * pow10(3);

    /* n >= 10 */
    idx_t k = 2 + (n - 10) / 2;
    if ((n & 1) == 0)    /* even n -> 10^(3k) */
        return pow10(3 * k);
    else                 /* odd n  -> 8*10^(3k) */
        return (seq_t)8 * pow10(3 * k);
}

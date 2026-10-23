/*
 * A179582  Number of permutations of 1..n+7 with deficiencies #{i: p(i) < i}
 *          exceeding the exceedances #{i: p(i) > i} by n or more.
 * https://oeis.org/A179582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+7)*(n+6)*(n^3-23*n^2-148*n+200)/120 * +
 *            8*(n+7)*(n^2-19*n-222)/3*2^n * + 729*(n+10)*3^n. * The
 *            implementation evaluates the three terms separately, * handles
 *            possible negative contributions of the first two terms * by
 *            erecting their absolute magnitudes, and finally performs * a
 *            signed s...
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 30, u128 n <= 40,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute powers of 2 and 3 */
    seq_t pow2n = pow2(n);
    seq_t pow3n = pow3(n);

    /* Signed intermediate arithmetic for small factors */
    __int128 sn = ( __int128 ) n;

    /* Term 1: (n+7)*(n+6)*(n^3-23*n^2-148*n+200)/120 */
    __int128 poly1 = sn*sn*sn - 23*sn*sn - 148*sn + 200;
    bool neg1 = poly1 < 0;
    __int128 abs_p1 = neg1 ? -poly1 : poly1;
    __int128 mag1_int = abs_p1 * (sn + 7) * (sn + 6) / 120;
    seq_t mag1 = (seq_t) mag1_int;

    /* Term 2: 8*(n+7)*(n^2-19*n-222)/3 * 2^n */
    __int128 poly2 = sn*sn - 19*sn - 222;
    bool neg2 = poly2 < 0;
    __int128 abs_p2 = neg2 ? -poly2 : poly2;
    __int128 small_factor2 = 8 * (sn + 7) * abs_p2 / 3;
    seq_t mag2 = (seq_t) small_factor2 * pow2n;

    /* Term 3: 729*(n+10)*3^n */
    __int128 small_factor3 = 729 * (sn + 10);
    seq_t mag3 = (seq_t) small_factor3 * pow3n;

    /* Combine positive and negative parts */
    seq_t sum_pos = mag3;
    seq_t sum_neg = 0;

    if (!neg1)
        sum_pos = sum_pos + mag1;
    else
        sum_neg = sum_neg + mag1;

    if (!neg2)
        sum_pos = sum_pos + mag2;
    else
        sum_neg = sum_neg + mag2;

    return sum_pos - sum_neg;
}

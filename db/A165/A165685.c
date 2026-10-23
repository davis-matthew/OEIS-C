/*
 * A165685  Dimension of the space of Siegel cusp forms of genus 2 and weight
 *          n.
 * https://oeis.org/A165685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n=3] + floor( (n^3 + 48*n^2 - 26*n - 495)/8640 - (n mod
 *            2)*(7*n^2 - 69*n)/576 + (((n+3) mod 4) - ((n+1) mod 4))*n/192 +
 *            (((n+5) mod 6) - ((n+3) mod 6))*n/216 + (n mod 3)/6 + (3*((n+3)
 *            mod 6) - 5*((n+2) mod 6) + 7*((n+1) mod 6) - 5*(n mod 6))/36 );
 *            (The indicator [n=3] adds 1 iff n equals 3.)
 * Range:     u8 n <= 117, u16 n <= 811, u32 n <= 9751, u64 n <= 9751,
 *            u128 n <= 9751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* convert index to signed long long for intermediate arithmetic */
    long long nn = (long long)n;
    long long N0 = 0;   /* numerator of the rational part before the floor */

    /* Term A: (n^3 + 48*n^2 - 26*n - 495)/8640 */
    N0 += nn*nn*nn + 48*nn*nn - 26*nn - 495;

    /* Term B: -(n mod 2)*(7*n^2 - 69*n)/576   multiplied by 8640/576 = 15 */
    long long m2   = nn & 1;     /* n mod 2 */
    long long B_num = -(m2 * (7*nn*nn - 69*nn));
    N0 += B_num * 15;

    /* Term C: (((n+3) mod 4) - ((n+1) mod 4))*n/192   multiplied by 8640/192 = 45 */
    long long m4_1 = (nn + 3) & 3;   /* (n+3) mod 4 */
    long long m4_2 = (nn + 1) & 3;   /* (n+1) mod 4 */
    long long C_num = (m4_1 - m4_2) * nn;
    N0 += C_num * 45;

    /* Term D: (((n+5) mod 6) - ((n+3) mod 6))*n/216   multiplied by 8640/216 = 40 */
    long long m6_1 = (nn + 5) % 6;
    long long m6_2 = (nn + 3) % 6;
    long long D_num = (m6_1 - m6_2) * nn;
    N0 += D_num * 40;

    /* Term E: (n mod 3)/6   multiplied by 8640/6 = 1440 */
    long long m3 = nn % 3;
    N0 += m3 * 1440;

    /* Term F: (3*((n+3) mod 6) - 5*((n+2) mod 6) + 7*((n+1) mod 6) - 5*(n mod 6))/36
               multiplied by 8640/36 = 240 */
    long long m6_3 = (nn + 3) % 6;
    long long m6_4 = (nn + 2) % 6;
    long long m6_5 = (nn + 1) % 6;
    long long m6_6 = nn % 6;
    long long F_num = 3*m6_3 - 5*m6_4 + 7*m6_5 - 5*m6_6;
    N0 += F_num * 240;

    /* Floor division of possibly negative N0 by 8640 */
    long long floor_val;
    if (N0 >= 0) {
        floor_val = N0 / 8640;
    } else {
        /* exact floor for negative values: floor(x/d) = - ((-x + d - 1)/d) */
        floor_val = - ((-N0 + 8639) / 8640);
    }

    /* Add the indicator [n=3] */
    if (nn == 3)
        floor_val += 1;

    return (seq_t)floor_val;
}

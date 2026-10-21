/*
 * A239575  Number of non-equivalent (mod D_3) ways to place 5
 *          indistinguishable points on a triangular grid of side n so that no
 *          two of them are adjacent.
 * https://oeis.org/A239575
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^10 + 5*n^9 - 130*n^8 - 310*n^7 + 7465*n^6 - 1336*n^5 -
 *            202980*n^4 + 464160*n^3 + 1783424*n^2 - 8360064*n +
 *            9192960)/23040 + IF(n is odd)*(25*n^4 - 94*n^3 - 418*n^2 +
 *            2053*n - 1779)/1536 The function evaluates the rational
 *            expression exactly using 128‑bit intermediate arithmetic and
 *            retu...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 25, u64 n <= 230, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;

    /* Convert n to 128‑bit for intermediate calculations. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* Pre‑compute powers of n up to n^10 (fits in 128‑bit for all n
       that produce a result fitting in seq_t). */
    unsigned __int128 pows[11];
    pows[0] = 1;
    for (idx_t i = 1; i <= 10; ++i)
        pows[i] = pows[i - 1] * nn;

    /* First polynomial (degree 10) */
    static const int exps1[11]   = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    static const int coeff1[11]  = { 1,  5,-130,-310,7465,-1336,-202980,464160,
                                    1783424,-8360064,9192960};

    unsigned __int128 sumPos0 = 0, sumNeg0 = 0;
    for (int i = 0; i < 11; ++i) {
        int exp = exps1[i];
        int   c  = coeff1[i];
        unsigned __int128 term = pows[exp] *
                                (c >= 0 ? (unsigned __int128)c : (unsigned __int128)(-c));
        if (c >= 0)
            sumPos0 += term;
        else
            sumNeg0 += term;
    }
    unsigned __int128 num0 = sumPos0 - sumNeg0;   /* n>=3 guarantees sumPos0>=sumNeg0 */

    /* Second polynomial (degree 4), included only for odd n */
    unsigned __int128 num1 = 0;
    if (n & 1) {
        static const int exps2[5]   = {4, 3, 2, 1, 0};
        static const int coeff2[5]  = {25, -94, -418, 2053, -1779};

        unsigned __int128 sumPos1 = 0, sumNeg1 = 0;
        for (int i = 0; i < 5; ++i) {
            int exp = exps2[i];
            int   c  = coeff2[i];
            unsigned __int128 term = pows[exp] *
                                    (c >= 0 ? (unsigned __int128)c : (unsigned __int128)(-c));
            if (c >= 0)
                sumPos1 += term;
            else
                sumNeg1 += term;
        }
        num1 = sumPos1 - sumNeg1;
    }

    /* Common denominator 23040*1536 = 35,388,240. */
    const unsigned __int128 DEN = (unsigned __int128)23040 * 1536;

    /* Total numerator after bringing to common denominator. */
    unsigned __int128 total_num = num0 * 1536 + num1 * 23040;

    unsigned __int128 result = total_num / DEN;   /* exact division */

    return (seq_t)result;
}

/*
 * A100213  Expansion of g.f.: x*(4-7*x+2*x^2-8*x^4+16*x^5-16*x^6)/((1-2*x) *
 *          (1-2*x^2) * (1-2*x+2*x^2) * (1+2*x^2)).
 * https://oeis.org/A100213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n = 2k-1 (odd): a(n) = 2^(k-3) * (2^(k+2) + 5 + (-1)^k -
 *            6*cos(kπ/2) + 4*sin(kπ/2)) For n = 2k (even): a(n) = 2^(k-2) *
 *            (2^(k+2) + 3 - 2*(-1)^k - cos(kπ/2) + 5*sin(kπ/2))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for offset 1, return 0 safely */
        return 0;

    if (n & 1ULL) {           /* odd n: n = 2k-1 */
        idx_t k = (n + 1) / 2;   /* k >= 1 */
        seq_t X = pow2(k + 2);   /* 2^(k+2) */

        /* add 5 + (-1)^k */
        if ((k & 1ULL) == 0)   /* k even: (-1)^k = 1  */
            X += 6;            /* 5 + 1 */
        else
            X += 4;            /* 5 - 1 */

        /* cos(kπ/2) & sin(kπ/2) via k mod 4 */
        idx_t mod4 = k & 3ULL;   /* k % 4 */
        if (mod4 == 0)          /* cos = +1  */
            X -= 6;             /* -6*cos = -6 */
        else if (mod4 == 2)     /* cos = -1  */
            X += 6;             /* -6*(-1) = +6 */

        if (mod4 == 1)          /* sin = +1  */
            X += 4;
        else if (mod4 == 3)     /* sin = -1  */
            X -= 4;

        /* factor 2^(k-3) */
        if (k >= 3)
            return X * pow2(k - 3);
        else if (k == 2)        /* divide by 2 */
            return X >> 1;
        else                    /* k == 1, divide by 4 */
            return X >> 2;
    }

    /* even n: n = 2k */
    idx_t k = n / 2;            /* k >= 1 */
    seq_t Y = pow2(k + 2);      /* 2^(k+2) */

    /* add 3 - 2*(-1)^k */
    if ((k & 1ULL) == 0)        /* k even: (-1)^k = 1 */
        Y += 1;                 /* 3 - 2*1 = 1 */
    else
        Y += 5;                 /* 3 - 2*(-1) = 5 */

    /* cos(kπ/2) & sin(kπ/2) via k mod 4 */
    idx_t mod4 = k & 3ULL;      /* k % 4 */
    if (mod4 == 0)              /* cos = +1  */
        Y -= 1;                 /* -cos = -1 */
    else if (mod4 == 2)         /* cos = -1  */
        Y += 1;                 /* -(-1) = +1 */

    if (mod4 == 1)              /* sin = +1  */
        Y += 5;
    else if (mod4 == 3)         /* sin = -1  */
        Y -= 5;

    /* factor 2^(k-2) */
    if (k >= 2)
        return Y * pow2(k - 2);
    else                        /* k == 1, divide by 2 */
        return Y >> 1;
}

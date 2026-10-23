/*
 * A182333  Number of arrangements of n bishops such that every square of the
 *          board is controlled by at least one bishop.
 * https://oeis.org/A182333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (((2*floor(n/4))!)^2/128) * ( n^5 + 3*n^4 + n^3 + 35*n^2
 *            + 38*n + 2 - (n^5 - n^4 - 7*n^3 - n^2 - 10*n - 30)*(-1)^n -
 *            4*(n^3 + 2*n^2 + n - 4)*n*cos(Pi*n/2) - 2*(n^5 + n^4 - 11*n^3 -
 *            7*n^2 - 2*n + 2)*sin(Pi*n/2) )
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* ---- factorial part ---- */
    /* compute (2*floor(n/4))!  and store its square in fact_sq */
    static idx_t cur_k = 0;              /* current floor(n/4) value */
    static seq_t fact_even = (seq_t)1;   /* (2*cur_k)! */

    idx_t k = n / 4;          /* floor(n/4) */

    if (k > cur_k) {
        for (idx_t m = cur_k + 1; m <= k; ++m) {
            /* multiply by (2m-1) and then by (2m) to extend factorial */
            fact_even = fact_even * (seq_t)(2 * m - 1);
            fact_even = fact_even * (seq_t)(2 * m);
        }
        cur_k = k;
    }

    seq_t fact_sq = fact_even * fact_even;   /* ((2k)!)^2 */

    /* ---- polynomial part ---- */
    __int128 n128 = ( __int128 ) n;
    __int128 n2   = n128 * n128;
    __int128 n3   = n2   * n128;
    __int128 n4   = n3   * n128;
    __int128 n5   = n4   * n128;

    __int128 term1 = n5
                    + (__int128)3 * n4
                    + n3
                    + (__int128)35 * n2
                    + (__int128)38 * n128
                    + 2;

    __int128 inner1   = n5
                       - n4
                       - (__int128)7   * n3
                       - n2
                       - (__int128)10  * n128
                       - 30;
    int    pow_neg1_n = (n & 1) ? -1 : 1;      /* (-1)^n */
    __int128 term2 = -inner1 * pow_neg1_n;

    /* cosine and sine signs depend on n mod 4 */
    int mod4 = (int)(n & 3U);                  /* n % 4 */
    int cosSign = 0;
    int sinSign = 0;
    switch (mod4) {
        case 0:  cosSign =  1; sinSign = 0; break;
        case 1:  cosSign =  0; sinSign =  1; break;
        case 2:  cosSign = -1; sinSign =  0; break;
        case 3:  cosSign =  0; sinSign = -1; break;
    }

    __int128 g   = n3 + (__int128)2 * n2 + n128 - 4;   /* n^3 + 2n^2 + n - 4 */
    __int128 term3 = -(__int128)4 * g * n128 * cosSign;

    __int128 h = n5 + n4
                 - (__int128)11 * n3
                 - (__int128)7  * n2
                 - (__int128)2  * n128
                 + 2;                        /* n^5 + n^4 - 11n^3 - 7n^2 - 2n + 2 */
    __int128 term4 = -(__int128)2 * h * sinSign;

    __int128 poly128 = term1 + term2 + term3 + term4;
    seq_t poly = (seq_t)poly128;        /* convert polynomial to seq_t (non‑negative) */

    /* ---- combine factorial part and polynomial without overflow ---- */
    seq_t denom = (seq_t)128ULL;

    /* cancel powers of two from factorial part first */
    seq_t g1 = gcd(fact_sq, denom);
    if (g1 != (seq_t)0) {
        fact_sq /= g1;
        denom   /= g1;
    }

    /* cancel remaining powers of two from polynomial part */
    seq_t g2 = gcd(poly, denom);
    if (g2 != (seq_t)0) {
        poly  /= g2;
        denom /= g2;
    }

    /* at this point denom should be 1, otherwise it must be 1 */
    seq_t result = fact_sq * poly;   /* final result */

    return result;
}

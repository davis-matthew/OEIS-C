/*
 * A027971  T(n, 2n-9), T given by A027960.
 * https://oeis.org/A027971
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 51, u64 n <= 575,
 *            u128 n <= 1000, big any n
 * Helpers:   binom9_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A027971
    Polynomial:  a(n) = (17055360 -16329024*n +7697736*n^2 -2299060*n^3
                        +462798*n^4 -60207*n^5 +4284*n^6 -30*n^7 -18*n^8
                        +n^9) / 362880
    Equivalent binomial formula
        a(n) = Σ_{j=0}^{min(9,n-5)} c[j] * binom(n+4-j , 9)
    where c = {3, -23, 83, -178, 253, -242, 157, -67, 17, -2}.
*/
#include <stdint.h>
#include "../utils/binom9_u128.h"

seq_t a(idx_t n)
{
    if (n < 5) return 0;           /* offset starts at 5 */

    /* Coefficients c[j] for the binomial expansion   */
    static const int c[10] = {3, -23, 83, -178, 253, -242,
                              157, -67, 17, -2};

    signed __int128 acc = 0;
    idx_t maxj = n - 5;
    if (maxj > 9) maxj = 9;

    for (idx_t j = 0; j <= maxj; ++j)
    {
        idx_t m = n + 4 - j;                 /* m = n+4-j  */
        unsigned __int128 prod = binom9_u128(m);
        signed __int128 term = (signed __int128)prod * (signed __int128)c[j];
        acc += term;
    }

    return (seq_t)acc;
}

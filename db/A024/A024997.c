/*
 * A024997  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is
 *          an integer, s(0) = 0 = s(n), |s(i) - s(i-1)| = 1 for i = 1,2;
 *          |s(i) - s(i-1)| <= 1 for i >= 3. Also a(n) = T(n,n), where T is
 *          the array defined in A024996.
 * https://oeis.org/A024997
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Sum_{k=0..floor((n-2)/2)} binomial(n-2,
 *            2*k)*binomial(2*k+1, k+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    idx_t m = n - 2;               /* use n-2 in the summation */
    seq_t sum = 0;

    for (idx_t k = 0; k <= m / 2; ++k)
    {
        seq_t c1 = binomial(m, 2 * k);            /* C(m, 2k)   */
        seq_t c2 = binomial(2 * k + 1, k + 1);    /* C(2k+1, k+1) */
        sum += c1 * c2;
    }

    return sum * ((seq_t)2);
}

/*
 * A158327  a(n) = A145444(n)-A145511(n).
 * https://oeis.org/A158327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for odd n; for even n set n = 2^v * m with m odd, then
 *            a(n) = 2 * ∏_{p^e || m} T_{e+1} where T_k = k*(k+1)/2
 * Range:     u8 n <= 5669, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* odd indices give 0 */
    if ((n & 1) == 1)
        return (seq_t)0;

    /* strip all factors of 2 */
    idx_t m = n;
    while ((m & 1U) == 0U)
        m >>= 1U;

    /* result starts with the constant factor 2 */
    seq_t result = (seq_t)2;

    /* factor the odd part m */
    idx_t temp = m;
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            /* exponent of p in m */
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                exp++;
            }

            /* triangular number T_{exp+1} = (exp+1)*(exp+2)/2 */
            seq_t tri = (seq_t)(exp + 1) * (seq_t)(exp + 2);
            tri /= (seq_t)2;

            result *= tri;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        /* exponent 1 */
        seq_t tri = (seq_t)(1 + 1) * (seq_t)(1 + 2); /* 2*3 = 6 */
        tri /= (seq_t)2;                           /* = 3 */
        result *= tri;                             /* 2 * 3 = 6 */
    }

    return result;
}

/*
 * A157056  Number of integer sequences of length n+1 with sum zero and sum of
 *          absolute values 14.
 * https://oeis.org/A157056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n,7) where T(n,k) = Σ_{i=1..n}
 *            C(n+1,i)*C(k-1,i-1)*C(n-i+k,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 79, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* values below the offset are irrelevant */

    seq_t res = 0;
    /* Since k-1 = 6, i never exceeds 7.  Limit the loop to min(n,7). */
    idx_t limit = n < 7 ? n : 7;

    for (idx_t i = 1; i <= limit; ++i)
    {
        seq_t c1 = binomial(n + 1, i);          /* C(n+1,i)      */
        seq_t c2 = binomial(6, i - 1);          /* C(6,i-1)      */
        seq_t c3 = binomial(n - i + 7, 7);      /* C(n-i+7,7)    */
        res += c1 * c2 * c3;
    }

    return res;
}

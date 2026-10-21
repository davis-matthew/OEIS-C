/*
 * A228273  T(n,k) is the number of s in {1,...,n}^n having longest ending
 *          contiguous subsequence with the same value of length k; triangle
 *          T(n,k), n>=0, 0<=k<=n, read by rows.
 * https://oeis.org/A228273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(m, k) where * m = largest integer with m(m+1)/2 <= n
 *            (i.e. floor((sqrt(8n+1)-1)/2)) * k = n - m(m+1)/2 (0 <= k <= m)
 *            * T(0,0) = 1 * T(m,0) = 0 for m > 0 * T(m,m) = m * T(m,k) =
 *            (m-1) * m^(m-k) for 1 <= k <= m-1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 153,
 *            u128 n <= 378, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Find m such that m(m+1)/2 <= n < (m+1)(m+2)/2 */
    idx_t m = 0;
    while (((m + 1) * (m + 2)) / 2 <= n)
        ++m;

    idx_t tri_m = (m * (m + 1)) / 2;
    idx_t k = n - tri_m;

    /* Evaluate T(m,k) according to the definition */
    if (m == 0)                  /* Only possible when n==0, already handled above */
        return (seq_t)1;
    if (k == 0)                  /* k=0 and m>0 -> 0 */
        return (seq_t)0;
    if (k == m)                  /* k=m -> value is m */
        return (seq_t)m;

    /* General case: (m-1) * m^(m-k) */
    seq_t base = (seq_t)m;
    seq_t exp  = (seq_t)(m - k);
    seq_t power = big_pow(base, exp);
    seq_t result = ((seq_t)(m - 1)) * power;
    return result;
}

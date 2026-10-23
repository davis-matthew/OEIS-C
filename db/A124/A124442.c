/*
 * A124442  a(n) = Product_{ceiling(n/2) <= k <= n, gcd(k,n)=1} k.
 * https://oeis.org/A124442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{ceil(n/2) <= k <= n, gcd(k,n)=1} k
 *            (Implementation: iterate over the interval, test gcd, multiply
 *            the coprime terms. Works for any seq_t/idx_t defined in the
 *            build.)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle any n < 1 just in case (undefined below offset). */
    if (n == 0)
        return (seq_t)0;

    /* Start at ceil(n/2).  For integer arithmetic, ceil(n/2) = (n+1)/2. */
    idx_t start = (n + 1) / 2;

    seq_t prod = (seq_t)1;

    for (idx_t k = start; k <= n; ++k)
    {
        /* gcd may be computed in seq_t; values fit into idx_t, so cast. */
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (g == (seq_t)1)
            prod *= (seq_t)k;
    }
    return prod;
}

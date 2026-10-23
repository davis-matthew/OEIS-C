/*
 * A199970  a(n) = the smallest number m with the smallest non-divisor n such
 *          that 1<n<m, or 0 if no such m exists.
 * https://oeis.org/A199970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m > n such that 1 < n < m, all k < n divide m, *
 *            but n does not divide m. Return 0 if no such m exists. * * For
 *            prime powers n = p^k (k ≥ 1) the answer is A003418(n‑1),
 *            otherwise 0. * * The implementation computes the LCM L =
 *            lcm(1,2,…,n-1) and then finds * the smallest multiple o...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* Compute L = lcm(1,2,…,n-1) */
    seq_t L = 1;
    for (idx_t i = 2; i < n; ++i) {
        seq_t g = gcd(L, (seq_t)i);
        L = (L / g) * (seq_t)i;
    }

    /* If n divides L, every multiple of L is divisible by n → no solution */
    if ((L % (seq_t)n) == 0)
        return 0;

    /* Find the smallest multiple of L that exceeds n and is not
     * divisible by n.  Since L is not divisible by n, this will
     * terminate quickly. */
    seq_t m = L;
    if (m <= n) {
        /* m <= n, so start with the smallest k so that L*k > n */
        idx_t k = (idx_t)((n / L) + 1);   /* ceil division with integer types */
        m = L * k;
    }

    /* m is already > n and L does not divide n, but just in case
     * of divisibility by n due to the chosen k, iterate until a
     * suitable m is found. */
    while ((m % (seq_t)n) == 0) {
        /* Increment k by 1 */
        idx_t k = (idx_t)(m / L) + 1;
        m = L * k;
    }

    return m;
}

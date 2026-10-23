/*
 * A130053  G.f. A(x) = (1-x+x^2)/(1-x)^3 - x*[Sum_{n>=0} x^(n +
 *          2^n)]/(1-x)^2.
 * https://oeis.org/A130053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1))/2 + 1 * - Σ_{m≥0, m + 2^m ≤ n-1} (n-1 - m - 2^m
 *            + 1)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* base term: (n^2 + n + 2)/2 = n*(n+1)/2 + 1 */
    seq_t base = ((seq_t)n * ((seq_t)n + 1)) / 2 + 1;

    /* t = n-1 */
    idx_t t = n - 1;
    seq_t sub = 0;

    /* sum over m with m + 2^m ≤ t */
    for (idx_t m = 0; m < 64; ++m)
    {
        idx_t pow2 = 1ULL << m;              /* 2^m  */
        idx_t s    = m + pow2;               /* m + 2^m */
        if (s > t)
            break;
        sub += (seq_t)(t - s + 1);            /* t - (m+2^m) + 1 */
    }

    return base - sub;
}

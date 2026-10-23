/*
 * A025531  a(n) = (2nd elementary symmetric function of {1/1, 1/2, ...,
 *          1/n})*(lcm(S))^2, where S = {1,2,...,n}.
 * https://oeis.org/A025531
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((∑_{i=1}^n (LCM(1..n)/i))^2 - ∑_{i=1}^n
 *            (LCM(1..n)/i)^2)/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the helper function gcd(seq_t, seq_t) from utils/gcd.h */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Compute L = lcm(1..n) */
    seq_t L = 1;
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t g = gcd(L, (seq_t)k);
        L = (L / g) * (seq_t)k;
    }

    /* Compute S = Σ(L/i) and T = Σ(L/i)^2 */
    seq_t S = 0, T = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = L / (seq_t)i;
        S += term;
        T += term * term;
    }

    /* a(n) = (S^2 - T)/2 */
    return (S * S - T) / 2;
}

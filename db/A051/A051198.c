/*
 * A051198  T(2n+5,n), array T as in A050186; a count of aperiodic binary
 *          words.
 * https://oeis.org/A051198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|gcd(2n+5,n)} μ(d) * C((2n+5)/d , n/d)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ is the Möbius function, C is the binomial coefficient.                       */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Greatest common divisor of two idx_t numbers. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Möbius function for idx_t values.  Works for all values that fit in idx_t. */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int mu = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            if (m % (p * p) == 0) return 0;   /* squared prime factor */
            mu = -mu;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) mu = -mu;   /* remaining prime factor */
    return mu;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t N = 2 * n + 5;   /* length of the word */
    idx_t K = n;          /* number of ones              */
    idx_t g = gcd_idx(N, K);   /* common divisor of N and K  */

    /* collect all divisors of g (g ≤ 20005, so at most ~128 divisors) */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            divs[divcnt++] = d;
            if (d != g / d) divs[divcnt++] = g / d;
        }
    }

    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];
        int mu = mobius(d);
        if (mu == 0) continue;

        seq_t term = binom(N / d, K / d);
        if (mu > 0) {
            sum += term;
        } else {
            sum -= term;
        }
    }
    return sum;
}

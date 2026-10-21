/*
 * A206771  0 followed by the numerators of the reduced (A001803(n) +
 *          A001790(n)) / (2*A046161(n)).
 * https://oeis.org/A206771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of n*binomial(2n-2, n-1)/4^(n-1) after
 *            cancelling common factors of 2. For n = 0, a(0) = 0. The
 *            algorithm uses a multiplicative evaluation of the binomial
 *            coefficient and Legendre’s formula for the exponent of 2 in
 *            factorials to avoid big‑integer division of the denominator
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Helper: exponent of 2 in m!  (Legendre's formula) */
static inline idx_t v2_factorial(idx_t m)
{
    /* v2(m!) = m - popcount(m) */
    return m - __builtin_popcountll(m);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset for the first term */
        return 0;

    /* Compute binomial(2n-2, n-1) multiplicatively */
    idx_t nm1 = n - 1;         /* n-1 */
    seq_t binom = 1;
    for (idx_t k = 1; k <= nm1; ++k)
    {
        binom = (binom * (seq_t)(nm1 + k)) / (seq_t)k;
    }

    /* Numerator before reduction: n * binom */
    seq_t N = binom * (seq_t)n;

    /* Exponent of 2 in the numerator */
    idx_t v2_n   = __builtin_ctzll((unsigned long long)n);        /* v2(n)   */
    idx_t v2_bom = v2_factorial(2*n - 2) - 2 * v2_factorial(nm1); /* v2(binomial) */
    idx_t v2_N   = v2_n + v2_bom;

    /* Denominator has 2^(2n-2); cancel min(v2_N, 2n-2) twos */
    idx_t shift = v2_N < (2*n - 2) ? v2_N : (2*n - 2);

    return N >> shift;          /* reduced numerator */
}

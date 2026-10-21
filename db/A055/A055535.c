/*
 * A055535  Denominators in expansion of (1-x)^(-1/x)/e.
 * https://oeis.org/A055535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055535: denominators in expansion of (1-x)^(-1/x)/e
 *
 * This implementation uses the recurrence derived from expanding
 * (1-x)^(-1/x) as a power series.  For each coefficient r_n =
 * p_n/q_n we compute q_n by
 *
 *   q_n = lcm_{k=0}^{n-1} ( (n-k+1) * q_k )  *  n
 *
 * and then reduce the fraction with the numerator produced in the
 * same loop.  The code stores all intermediate numerator and
 * denominator values in static caches to allow arbitrary ordering of
 * calls.  GCD is computed with a small Euclidean routine that works
 * for all integer types provided by the build.
 */

#include <stdlib.h>

static seq_t gcd_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static caches for numerators (p_i) and denominators (q_i) */
    static seq_t *num = NULL;
    static seq_t *den = NULL;
    static idx_t cap = 0;

    /* Ensure capacity for index n */
    if (n >= cap) {
        idx_t newcap = cap ? cap << 1 : 4;
        while (newcap <= n) newcap <<= 1;
        num = (seq_t *)realloc(num, newcap * sizeof(seq_t));
        den = (seq_t *)realloc(den, newcap * sizeof(seq_t));
        for (idx_t i = cap; i < newcap; ++i) {
            num[i] = 0;
            den[i] = 0;
        }
        cap = newcap;
    }

    /* Already computed? */
    if (den[n] != 0) return den[n];

    /* Base case a(0) = 1 / 1 */
    if (n == 0) {
        num[0] = 1;
        den[0] = 1;
        return den[0];
    }

    /* Compute values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (den[i] != 0) continue;          /* already done */

        /* Compute L = lcm_{j=0}^{i-1} ( (i-j+1) * den[j] ) */
        seq_t L = 1;
        for (idx_t j = 0; j < i; ++j) {
            seq_t term = (seq_t)(i - j + 1) * den[j];
            seq_t g = gcd_local(L, term);
            L = (L / g) * term;
        }

        /* Compute R = Σ_{j=0}^{i-1} (i-j) * num[j] * (L / ((i-j+1)*den[j])) */
        seq_t R = 0;
        for (idx_t j = 0; j < i; ++j) {
            seq_t denom_factor = (seq_t)(i - j + 1) * den[j];
            seq_t factor = L / denom_factor;           /* integer division */
            seq_t term = factor * num[j];
            term *= (seq_t)(i - j);
            R += term;
        }

        /* r_i = R / (i * L), reduce the fraction */
        seq_t mult = (seq_t)i * L;
        seq_t g = gcd_local(R, mult);
        den[i] = mult / g;
        num[i] = R / g;          /* numerator for future steps */
    }

    return den[n];
}

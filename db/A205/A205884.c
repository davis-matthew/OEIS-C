/*
 * A205884  a(n) = Pell(n)*A109064(n) for n >= 1 with a(0)=1.
 * https://oeis.org/A205884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Pell(n) * (-5 * Σ_{d|n} d * L(d,5)) for n ≥ 1, a(0) = 1.
 *            Here Pell(n) is the Pell sequence (Pell(0)=0, Pell(1)=1) and
 *            L(d,5) is the Legendre symbol (d|5)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 20, i64 n <= 45, i128 n <= 95,
 *            big any n
 * Helpers:   legendre5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/legendre5.h"

/* Integer square root, floor(sqrt(n)).  Works for all uint64_t. */
static inline idx_t int_sqrt_u64(idx_t n)
{
    if (n < 2ULL) return n;
    long double x = sqrtl((long double)n);
    idx_t s = (idx_t)x;
    /* Adjust to guarantee floor. */
    while ((s + 1ULL) * (s + 1ULL) <= n) ++s;
    while (s * s > n) --s;
    return s;
}

/* Compute a(n) from A205884.  Works for all supported seq_t types. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* ----- Pell(n) ----- */
    seq_t pell_prev2 = (seq_t)0;          /* Pell(0) */
    seq_t pell_prev1 = (seq_t)1;          /* Pell(1) */
    seq_t pell_n = (seq_t)0;
    if (n == 0) pell_n = pell_prev2;
    else if (n == 1) pell_n = pell_prev1;
    else {
        for (idx_t i = 2; i <= n; ++i) {
            pell_n = (seq_t)2 * pell_prev1 + pell_prev2;
            pell_prev2 = pell_prev1;
            pell_prev1 = pell_n;
        }
    }

    /* ----- Σ_{d|n} d * L(d,5) ----- */
    seq_t sum = (seq_t)0;
    idx_t limit = int_sqrt_u64(n);
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            int l = legendre5(d);
            if (l != 0) sum += (seq_t)d * (seq_t)l;

            idx_t other = n / d;
            if (other != d) {
                int l2 = legendre5(other);
                if (l2 != 0) sum += (seq_t)other * (seq_t)l2;
            }
        }
    }

    /* ----- Combine ----- */
    seq_t mult = (seq_t)(-5) * sum;   /* -5 * Σ_{d|n} d*L(d,5) */
    return pell_n * mult;
}

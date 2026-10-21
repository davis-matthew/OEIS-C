/*
 * A221214  O.g.f.: Sum_{n>=0} (3*n+1)^(3*n+1) * exp(-(3*n+1)^3*x) * x^n / n!.
 * https://oeis.org/A221214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n! * Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) *
 *            (3*k+1)^(3*n+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t exp = (seq_t)(3 * n + 1);

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base   = (seq_t)(3 * k + 1);
        seq_t pow_val = big_pow(base, exp);
        seq_t term = binomial(n, k) * pow_val;

        if ((n - k) & 1)      /* (-1)^(n-k) == -1 */
            sum -= term;
        else                  /* (+1) */
            sum += term;
    }

    seq_t fact = factorial(n);
    return sum / fact;        /* exact division */
}

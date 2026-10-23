/*
 * A168481  G.f.: Sum_{n>=0} (n+1)*2^(n^2)*(1 + 2^n*x)^n*x^n.
 * https://oeis.org/A168481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..floor(n/2)} (n-k+1) * C(n-k,k) * 2^(n*(n-k))
 * (The values are returned as seq_t; overflow is modulo 2^width of seq_t as
 * defined by the build. For the arbitrary-precision build all terms are
 * exact. For small builds only the terms that fit in seq_t are correct.)
 */

#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include "../utils/pow2.h"       /* pow2(idx_t) */

seq_t a(idx_t n)
{
    seq_t total = 0;

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t nk = n - k;                      /* n - k */
        seq_t coeff = (seq_t)(nk + 1);          /* n-k+1 */

        /* Compute binomial C(n-k, k) */
        seq_t binom = binomial(nk, k);

        /* Exponent: n*(n-k) */
        idx_t exp = n * nk;
        seq_t power = pow2(exp);

        /* term = coeff * binom * power */
        total += coeff * binom * power;
    }

    return total;
}

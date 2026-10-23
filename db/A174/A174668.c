/*
 * A174668  a(n) is the number of solutions of the congruence Sum_{k=1..n} x_k
 *          == n (mod 2n) where x_k are distinct elements of the set
 *          {0,1,...,2n}.
 * https://oeis.org/A174668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)!/2) * Σ_{d|n} (-1)^(n+d) * φ(n/d) * C(2d,d)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binom_n_k, calc_phi, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/calc_phi.h"
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* factorial part: (n-1)! */
    seq_t fact = factorial(n - 1);

    /* sums for positive and negative terms */
    seq_t pos = 0;
    seq_t neg = 0;

    /* enumerate divisors of n */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor: d */
            idx_t other = n / d;
            seq_t phi_val = calc_phi((seq_t)other);
            seq_t bin_val = binom_n_k(2 * d, d);
            seq_t term = phi_val * bin_val;
            if (((n + d) & 1ULL) == 0)
                pos += term;
            else
                neg += term;

            /* second divisor: other, if distinct */
            if (other != d)
            {
                idx_t d2 = other;                /* d2 = n/d */
                seq_t phi_val2 = calc_phi((seq_t)(n / d2)); /* = phi(d) */
                seq_t bin_val2 = binom_n_k(2 * d2, d2);
                seq_t term2 = phi_val2 * bin_val2;
                if (((n + d2) & 1ULL) == 0)
                    pos += term2;
                else
                    neg += term2;
            }
        }
    }

    /* S = pos - neg  (S is always >= 0) */
    seq_t S = pos;
    if (S >= neg)
        S -= neg;
    else
        S = 0; /* should not happen, just for safety */

    /* full product = fact * S, then divide by 2 */
    seq_t prod = fact * S;
    seq_t result = prod / 2;
    return result;
}

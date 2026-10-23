/*
 * A173129  a(n) = cosh(2 * n * arccosh(n)).
 * https://oeis.org/A173129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2n,2k) * (n^2-1)^(n-k) * n^(2k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Return a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Convert the index to the integer type used for values. */
    seq_t nn = (seq_t)n;

    /* Compute the common base (n^2 - 1). */
    seq_t base = nn * nn - (seq_t)1;

    /* pow1 = (n^2-1)^n  ,  pow2 = n^(2k)  (kept as (n^2)^k). */
    seq_t pow1 = big_pow(base, (seq_t)n);
    seq_t pow2 = (seq_t)1;

    seq_t sum = (seq_t)0;

    /* Iterate over k = 0 .. n.  During each step pow1 is divided by
       (n^2-1) and pow2 is multiplied by (n^2), except after the last
       iteration. */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(2 * n, 2 * k);

        /* term = bin * pow1 * pow2 */
        seq_t term = bin * pow1;
        term = term * pow2;

        sum += term;

        if (k < n) {
            pow1 /= base;           /* exact division: (n^2-1)^(n-k-1)   */
            pow2 *= nn * nn;        /* (n^2)^ (k+1) = n^(2(k+1))   */
        }
    }

    return sum;
}

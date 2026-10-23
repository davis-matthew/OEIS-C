/*
 * A038053  Number of labeled planted trees with 2-colored leaves.
 * https://oeis.org/A038053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{k=0}^{n-1} binom(n-1, k) * k^{n-2} (for n ≥ 2)
 *            a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standards */
#include <stdint.h>

/* Helper functions provided by the environment */
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/seq_pow.h" /* seq_t seq_pow(seq_t base, idx_t exp); */

/* Function to compute the n-th term of OEIS sequence A038053 */
seq_t a(idx_t n)
{
    /* Offset 1 : a(1) = 0 */
    if (n <= 1)
        return (seq_t)0;

    idx_t m = n - 1;                 /* m = n-1 */
    idx_t p = m - 1;                 /* exponent for k^{p} where p = n-2 */

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= m; ++k)
    {
        /* binom(m, k) */
        seq_t b = binom(m, k);

        /* k^p */
        seq_t pow_k = seq_pow((seq_t)k, p);

        /* term = binom(m, k) * k^{p} */
        seq_t term = b * pow_k;

        /* accumulate */
        sum += term;
    }

    /* a(n) = n * sum */
    return sum * (seq_t)n;
}

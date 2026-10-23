/*
 * A025585  Central Eulerian numbers A(2n-1,n).
 * https://oeis.org/A025585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} (-1)^j * (n-j)^(2n-1) * C(2n,j)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Central Eulerian numbers A025585: A(2n-1,n) */

#include "../utils/big_pow.h"   /* big_pow(base, exp) -> seq_t */
#include "../utils/binomial.h" /* binomial(n, k)   -> seq_t */

/* External linkage as required by the build system */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n == 0 return 0 to keep
       the function well‑defined in all call sites. */
    if (n == 0)
        return 0;

    /* Pre‑compute the exponent 2n-1 once. */
    seq_t exp = (seq_t)(2 * n - 1);

    seq_t sum = 0;  /* running sum of the terms with alternating signs */

    for (idx_t j = 0; j <= n; ++j) {
        /* (n-j) as a seq_t. */
        seq_t base = (seq_t)(n - j);

        /* (n-j)^(2n-1) */
        seq_t power = big_pow(base, exp);

        /* binomial coefficient C(2n,j) */
        seq_t comb = binomial(2 * n, j);

        /* product of power and combinatorial factor */
        seq_t term = power * comb;

        /* add or subtract according to the parity of j */
        if (j & 1)
            sum -= term;
        else
            sum += term;
    }

    return sum;
}

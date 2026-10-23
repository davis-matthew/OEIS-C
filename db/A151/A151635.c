/*
 * A151635  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 5 adjacent element pairs in decreasing order.
 * https://oeis.org/A151635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..7} (-1)^(j+1) * binomial(3*n+1, 7-j) *
 *            (binomial(j+1, 3))^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, small_comb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* for seq_t big_pow(seq_t, seq_t) */
#include "../utils/small_comb.h"

seq_t a(idx_t n)
{
    /* For n >= 1, compute the formula.  Intermediate positives
       and negatives are accumulated separately to keep subtraction
       with unsigned seq_t safe. */
    seq_t pos = 0;
    seq_t neg = 0;

    /* Pre‑compute 3*n+1 once, it is needed for each j. */
    idx_t big_n_part = 3 * n + 1;

    for (idx_t j = 0; j <= 7; ++j)
    {
        int sign = (j % 2 == 0) ? -1 : 1;   /* (-1)^(j+1) */
        seq_t base = small_comb(j + 1, 3);  /* (j+1 choose 3) */

        /* Skip terms that are immediately zero: base = 0 or base^n = 0 */
        if (base == 0)
            continue;

        /* Compute base^n */
        seq_t pow_term;
        if (base == 1)
            pow_term = 1;                  /* 1^n = 1 */
        else
            pow_term = big_pow(base, (seq_t)n);

        /* binomial(3*n+1, 7-j) */
        idx_t k = 7 - j;
        seq_t binom_term = small_comb(big_n_part, k);

        seq_t term = binom_term * pow_term;

        if (sign > 0)
            pos += term;
        else
            neg += term;
    }

    /* The final result is pos - neg; mathematically pos >= neg. */
    return pos - neg;
}

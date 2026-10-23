/*
 * A151629  Number of permutations of 2 indistinguishable copies of 1..n with
 *          exactly 7 adjacent element pairs in decreasing order.
 * https://oeis.org/A151629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..7} (-1)^j * binomial(2*n+1, j) * *
 *            binomial(9-j, 2)^n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t even_sum = 0, odd_sum = 0;
    /* Pre‑compute the binomial bases for the powers: */
    const seq_t base_tbl[8] = { binomial(9, 2),   /* 36 */
                                binomial(8, 2),   /* 28 */
                                binomial(7, 2),   /* 21 */
                                binomial(6, 2),   /* 15 */
                                binomial(5, 2),   /* 10 */
                                binomial(4, 2),   /* 6  */
                                binomial(3, 2),   /* 3  */
                                binomial(2, 2) }; /* 1  */

    for (idx_t j = 0; j <= 7; ++j) {
        seq_t binom_part = binomial(2*n + 1, j);
        if (binom_part == 0) {
            /* j > 2*n+1 ⇒ binomial is zero; skip the term */
            continue;
        }
        seq_t pow_term = big_pow(base_tbl[j], (seq_t)n);
        seq_t term = binom_part * pow_term;
        if (j & 1U)
            odd_sum += term;
        else
            even_sum += term;
    }
    return even_sum - odd_sum;
}

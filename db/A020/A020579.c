/*
 * A020579  Expansion of g.f. 1/((1-6*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A020579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n - 32*8^n + 27*9^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the necessary powers of 6, 8, and 9. */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);

    /* Scale each power by its coefficient. */
    seq_t term6 = ((seq_t)6) * pow6;
    seq_t term8 = ((seq_t)32) * pow8;
    seq_t term9 = ((seq_t)27) * pow9;

    /* Combine the terms as specified by the closed‑form formula. */
    return term6 + term9 - term8;
}

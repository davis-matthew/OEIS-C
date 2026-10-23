/*
 * A027816  a(n) = 66*(n+1)*binomial(n+5,11).
 * https://oeis.org/A027816
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*(n+1)*binomial(n+5,11)
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 122, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute C(n+5,11) multiplicatively */
    seq_t binom = 1;
    idx_t i;
    /* n should be >=6 for a valid term; for n<6 binom will be 0 essentially */
    for (i = 1; i <= 11; ++i) {
        seq_t numerator = (seq_t)(n - 6) + i;   /* (n-6+i) */
        binom = binom * numerator / (seq_t)i;
    }

    seq_t result = binom;
    result *= (seq_t)66;
    result *= ((seq_t)n + 1);
    return result;
}

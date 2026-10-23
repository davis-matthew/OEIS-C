/*
 * A027823  a(n) = 77*(n+1)*binomial(n+6,11).
 * https://oeis.org/A027823
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 77*(n+1)*binomial(n+6,11)
 * Range:     u8 none, u16 n <= 7, u32 n <= 18, u64 n <= 119, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* stdint types for potential use; seq_t/idx_t are defined elsewhere */

seq_t a(idx_t n)
{
    /* compute binomial(n+6,11) multiplicatively */
    seq_t binom = 1;
    idx_t k = 11;
    for (idx_t i = 0; i < k; ++i)
    {
        binom = binom * ((seq_t)(n + 6 - i)) / ((seq_t)(i + 1));
    }

    /* compute 77 * (n + 1) * binom */
    seq_t result = binom * ((seq_t)77 * (seq_t)(n + 1));
    return result;
}

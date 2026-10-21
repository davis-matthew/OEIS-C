/*
 * A052482  a(n) = 2^(n-2)*binomial(n+1,2).
 * https://oeis.org/A052482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * binomial(n+1,2) = (n*(n+1)/2) * 2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is not defined – return 0. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Compute binomial(n+1,2) = n*(n+1)/2 with seq_t arithmetic. */
    seq_t binom = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Compute 2^(n-2) by repeated left shift; safe for big‑int types. */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n - 2; ++i) {
        pow2 <<= 1;
    }

    /* Final result: binom * 2^(n-2) */
    return binom * pow2;
}

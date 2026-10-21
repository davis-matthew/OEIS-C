/*
 * A050182  a(n) = T(2*n+4, n), array T as in A051168 (a count of Lyndon
 *          words).
 * https://oeis.org/A050182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = T(2*n+4, n) * T(2*n+4, n) = binomial(2*n + 4, n)/(2*n +
 *            4) if n is odd * T(2*n+4, n) = (binomial(2*n + 4, n) -
 *            binomial(n + 2, n/2)) * /(2*n + 4) if n is even * * This
 *            implements the closed‐form formula directly, using the utility *
 *            function binom(idx_t a, idx_t b) from the shared utilities
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute the nth term of OEIS sequence A050182. */
seq_t a(idx_t n)
{
    /* Compute the main binomial coefficient binom(2*n+4, n). */
    idx_t two_n_plus_4 = 2*n + 4;
    seq_t top = binom(two_n_plus_4, n);

    /* For even n subtract binom(n+2, n/2). */
    if ((n & 1) == 0) {  /* n is even */
        idx_t half = n / 2;
        seq_t sub   = binom(n + 2, half);
        top -= sub;
    }

    /* Divide by (2*n + 4).  The division is exact by definition. */
    seq_t denom = (seq_t)two_n_plus_4;
    seq_t result = top / denom;

    return result;
}

/*
 * A141222  Expansion of -1/(2*x) + (2*x-1)^2/(2*x*(1-4x)^(3/2)).
 * https://oeis.org/A141222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + 3*n + 1) * C(2*n, n) / (n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative recurrence: (n+1)*(n^2+n-1)*a(n) = 2*(2*n-1)*(n^2+3*n+1)*a(n-1) */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    seq_t n_seq = (seq_t)n;

    /* Compute binomial coefficient C(2*n, n) */
    seq_t bin = binom((idx_t)(2 * n), n);

    /* Catalan number: C(2*n, n)/(n+1) */
    seq_t catalan = bin / (n_seq + (seq_t)1);

    /* Factor (n^2 + 3*n + 1) */
    seq_t factor = n_seq * n_seq + (seq_t)3 * n_seq + (seq_t)1;

    return catalan * factor;
}

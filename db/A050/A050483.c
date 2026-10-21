/*
 * A050483  Partial sums of A051947.
 * https://oeis.org/A050483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+6, 6)*(4*n+7)/7 (OEIS A050483)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 62, u64 n <= 1564,
 *            u128 n <= 10000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Returns the n-th term of sequence A050483 with offset 0. */
seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+6,6). */
    seq_t res = binom(n + 6, 6);

    /* Compute the multiplier (4*n + 7) / 7 without overflow. */
    seq_t num = (seq_t)(4 * (seq_t)n + 7);

    if ((num % 7u) == 0) {
        /* (4n+7) is divisible by 7: multiply directly. */
        res = res * (num / 7u);
    } else {
        /* Otherwise C(n+6,6) must be divisible by 7: divide first. */
        res = (res / 7u) * num;
    }

    return res;
}

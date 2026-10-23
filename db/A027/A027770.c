/*
 * A027770  a(n) = (n + 1)*binomial(n + 1, 10).
 * https://oeis.org/A027770
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1) * binomial(n + 1, 10)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 32, u64 n <= 225,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Valid for n >= 9 only; below the offset the value is undefined. */
    if (n < 9) return (seq_t)0;
    /* Compute the binomial coefficient C(n+1, 10) in seq_t and multiply. */
    seq_t C = binomial(n + 1, 10);
    return C * ((seq_t)n + 1);
}

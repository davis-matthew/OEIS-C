/*
 * A052254  Partial sums of A050406.
 * https://oeis.org/A052254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10*n + 7)*binomial(n+6, 6)/7
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+6, 6). */
    seq_t b = binomial(n + 6, 6);

    /* Compute (10*n + 7) * b and divide by 7.  Division is exact. */
    seq_t term = ((seq_t)10 * (seq_t)n + (seq_t)7) * b;
    return term / (seq_t)7;
}

/*
 * A083073  A subdiagonal of number array A083064.
 * https://oeis.org/A083073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+3)*(n+5)^n + 1)/(n+4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* Returns the n-th term of OEIS A083073. */
seq_t a(idx_t n)
{
    /* Compute (n+5)^n using the provided fast exponentiation routine. */
    seq_t base = (seq_t)(n + 5);
    seq_t pw = pow_seq(base, (int)n);

    /* Numerator: (n+3)*(n+5)^n + 1 */
    seq_t numerator = (seq_t)(n + 3) * pw + 1;

    /* Denominator: (n+4) */
    seq_t denom = (seq_t)(n + 4);

    /* Exact division (the result is always integer). */
    return numerator / denom;
}

/*
 * A119582  a(n) = (n^2+n^3)*(binomial(2*n,n))/2.
 * https://oeis.org/A119582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + n^3) * (binomial(2*n, n)) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial helper from the utils library. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* cast n to seq_t for exact arithmetic */
    seq_t nn = (seq_t)n;

    /* compute n^2 + n^3 */
    seq_t coeff = nn * nn;        /* n^2 */
    coeff = coeff + (nn * nn * nn); /* + n^3 */

    /* divide by 2 (always integer) */
    coeff = coeff / 2;

    /* binomial coefficient C(2n, n) */
    seq_t c = binomial(2 * n, n);

    /* final result */
    return coeff * c;
}

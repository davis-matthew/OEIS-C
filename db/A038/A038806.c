/*
 * A038806  Convolution of A008549 with A000302 (powers of 4).
 * https://oeis.org/A038806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*4^n - (n+2)*binomial(2*n+3, n+1)/2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow4.h"

/* Compute term a(n) of OEIS sequence A038806.
 * Uses the closed-form formula.
 * All arithmetic is performed in the arbitrary-precision type seq_t,
 * avoiding intermediate overflow of built-in integer types. */
seq_t a(idx_t n)
{
    seq_t term1, term2, bin, val;
    /* term1 = (n + 3) * 4^n */
    term1 = (seq_t)(n + 3) * pow4(n);

    /* bin = binomial(2*n + 3, n + 1) */
    bin = binomial((idx_t)(2 * n + 3), (idx_t)(n + 1));

    /* term2 = ((n + 2) * bin) / 2 */
    term2 = ((seq_t)(n + 2) * bin) / 2;

    /* result = term1 - term2 */
    val = term1 - term2;
    return val;
}

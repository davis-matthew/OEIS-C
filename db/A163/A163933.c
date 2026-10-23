/*
 * A163933  Third right hand column of triangle A163932.
 * https://oeis.org/A163933
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*binom(n+3,6) + 28*binom(n+2,6) + 6*binom(n+1,6) * for
 *            n >= 3. The binomial function is provided by the build *
 *            environment; it returns a value of type seq_t. All arithmetic is
 *            * performed in seq_t, which handles the required bit width or *
 *            arbitrary‑precision
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 64, u64 n <= 999, u128 n <= 999,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides binom(idx_t, idx_t) */

seq_t a(idx_t n) {
    if (n < 3) return 0;   /* values below the offset are undefined,
                              but this keeps the function safe. */
    /* Compute binomial coefficients.  All indices are idx_t;
       the binom function returns seq_t. */
    seq_t c1 = binom(n + 3, 6);   /* C(n+3,6) */
    seq_t c2 = binom(n + 2, 6);   /* C(n+2,6) */
    seq_t c3 = binom(n + 1, 6);   /* C(n+1,6) */

    /* Combine with the constant factors.  The intermediate results
       fit in seq_t: the sequence increases polynomially (degree 6),
       so values are within bounds of seq_t up to the required limits. */
    return 11 * c1 + 28 * c2 + 6 * c3;
}

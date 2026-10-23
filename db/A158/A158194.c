/*
 * A158194  a(n) = Sum_{i=1..n-1} (-1)^i*binomial(n, i-1)*binomial(n,
 *          i)*binomial(n, i+1).
 * https://oeis.org/A158194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{i=1..n-1} (-1)^i * binomial(n, i-1) * binomial(n, i)
 *            * binomial(n, i+1) Equivalent closed form: a(2*m) = 2*(-1)^m *
 *            binomial(2*m, m-1) * binomial(3*m, m-1), and a(2*m-1) = 0. This
 *            implementation uses the supplied binomial() and seq_mul()
 *            helpers to avoid overflow in the 64‑bit builds...
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 29, i128 n <= 57,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"     /* provides binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/seq_mul.h"   /* provides seq_mul(seq_t a, seq_t b) -> seq_t */

seq_t a(idx_t n)
{
    /* Odd indices produce zero */
    if (n & 1)
        return 0;

    /* n is even: n = 2*m */
    idx_t m = n >> 1;

    /* Compute the two binomial coefficients */
    seq_t c1 = binom( 2 * m, m - 1 );   /* binom(2*m, m-1) */
    seq_t c2 = binom( 3 * m, m - 1 );   /* binom(3*m, m-1) */

    /* Multiply them with 2 using seq_mul to keep it exact */
    seq_t prod = seq_mul( c1, c2 );
    prod = seq_mul( prod, (seq_t)2 );

    /* Apply the sign (-1)^m */
    if (m & 1)
        prod = seq_mul( prod, (seq_t)(-1) );

    return prod;
}

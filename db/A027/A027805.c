/*
 * A027805  a(n) = 21*(n+1)*binomial(n+4,9).
 * https://oeis.org/A027805
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21 * (n+1) * C(n+4, 9)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 24, u64 n <= 223, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+4, 9).  For n < 5 the
       function will correctly return 0 because 9 > n+4.  No special
       handling for the offset is required. */
    seq_t val = binomial(n + 4, 9);

    /* Multiply by (n+1) and the constant 21.  All intermediate
       results stay within seq_t, which may be an unsigned integer
       of any supported size or an arbitrary‑precision integer. */
    val = val * (seq_t)(n + 1);
    val = val * 21;

    return val;
}

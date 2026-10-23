/*
 * A027817  a(n) = 99*(n+1)*binomial(n+5,12).
 * https://oeis.org/A027817
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*(n+1)*binomial(n+5,12)
 * Range:     u8 none, u16 n <= 8, u32 n <= 18, u64 n <= 99, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 7. For smaller indices
       the value is not defined; return 0 in that case. */
    if (n < 7)
        return (seq_t)0;

    /* Compute binomial(n+5,12). */
    seq_t b = binomial(n + 5, (idx_t)12);

    /* Multiply by 99*(n+1). All arithmetic is performed in seq_t
       to preserve precision for arbitrary‑precision builds. */
    seq_t result = (seq_t)99 * ((seq_t)(n + 1)) * b;
    return result;
}

/*
 * A027776  a(n) = (n+1)*binomial(n+1,16).
 * https://oeis.org/A027776
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * C(n+1, 16) (with offset 15)
 * Range:     u8 n <= 15, u16 n <= 18, u32 n <= 28, u64 n <= 88,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Return the nth term of OEIS sequence A027776. */
seq_t a(idx_t n)
{
    idx_t m = n + (idx_t)1;          /* m = n+1 */
    seq_t binom_val = binom(m, (idx_t)16);  /* C(m, 16) */
    return (seq_t)m * binom_val;    /* (n+1) * C(n+1,16) */
}

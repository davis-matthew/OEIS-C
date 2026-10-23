/*
 * A027772  a(n) = (n+1)*binomial(n+1,12).
 * https://oeis.org/A027772
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,12)
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 29, u64 n <= 145,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 11.
       For smaller n we return 0 as a safe dummy value. */
    if (n < 11)
        return (seq_t)0;

    idx_t m = n + 1;                /* m = n+1  */
    seq_t bin = binomial(m, (idx_t)12); /* C(m,12) */
    return bin * (seq_t)m;           /* (n+1)*C(n+1,12) */
}

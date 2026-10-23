/*
 * A027802  a(n) = 5*(n+1)*binomial(n+4,6).
 * https://oeis.org/A027802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+1)*binomial(n+4,6)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* sequence is defined for n ≥ 2 */

    seq_t result = binomial(n + 4, 6);   /* compute C(n+4,6) in seq_t */
    result = result * ((seq_t)n + 1);    /* multiply by (n+1) with seq_t cast */
    result = result * (seq_t)5;          /* multiply by 5 */
    return result;
}

/*
 * A027768  a(n) = (n+1)*binomial(n+1,8).
 * https://oeis.org/A027768
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,8)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* values are defined for n >= 7; below that the function returns 0 */
    if (n < 7)
        return 0;

    seq_t b = binomial(n + 1, 8);
    return ((seq_t)(n + 1)) * b;
}

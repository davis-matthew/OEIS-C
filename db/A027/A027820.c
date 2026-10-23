/*
 * A027820  a(n) = 28*(n+1)*binomial(n+6,8)/3.
 * https://oeis.org/A027820
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1,3)*C(n+6,6)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        /* Sequence is defined for n ≥ 2. */
        return 0;
    }
    seq_t c1 = binom(n + 1, 3);
    seq_t c2 = binom(n + 6, 6);
    return c1 * c2;
}

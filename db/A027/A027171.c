/*
 * A027171  a(n) = A027170(2n, n).
 * https://oeis.org/A027171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(2*n+2, n+1) + binomial(2*n, n) - 4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 2*binomial(2*n+2, n+1) */
    seq_t t1 = binomial(2*n + 2ULL, n + 1ULL);
    t1 *= 2;

    /* Compute binomial(2*n, n) */
    seq_t t2 = binomial(2*n, n);

    /* Result: t1 + t2 - 4 */
    return t1 + t2 - 4;
}

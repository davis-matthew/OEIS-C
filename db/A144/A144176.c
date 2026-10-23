/*
 * A144176  a(n) = abs(b(2n-1) - b(2n)) where b(3n+1) = floor(n/9) + 2,
 *          b(3n+2) = (n mod 9) + 2, b(3n+3) = b(3n+1)*b(3n+2) for n >= 0.
 * https://oeis.org/A144176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = abs(b(2n-1) - b(2n)) where b(3k+1) = floor(k/9) + 2,
 *            b(3k+2) = (k mod 9) + 2, b(3k+3) = b(3k+1) * b(3k+2) (k ≥ 0)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   b_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_at.h"

/* Sequence a(n) as defined in the OEIS entry */
seq_t
a(idx_t n)
{
    /* n is 1‑based; 2*n could reach the upper bound of idx_t for very large n, but
       the routine is intended for n up to at least 10000. */
    seq_t b1 = b_at(2U * n - 1U);
    seq_t b2 = b_at(2U * n);
    return (b1 > b2) ? (b1 - b2) : (b2 - b1);
}

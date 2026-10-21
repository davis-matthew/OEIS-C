/*
 * A095662  Seventh column (m=6) of (1,3)-Pascal triangle A095660.
 * https://oeis.org/A095662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) * (n+18) / 6
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+5, 5) as a seq_t */
    seq_t bin = binomial(n + 5, 5);

    /* Compute (n + 18) as a seq_t */
    seq_t add = (seq_t)n + 18;

    /* Final result: bin * (n+18) / 6 (exact division) */
    seq_t res = (bin * add) / 6;

    return res;
}

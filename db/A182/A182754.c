/*
 * A182754  a(1) = 1, a(2) = 21, a(n) = 77*a(n-2) for n>=3.
 * https://oeis.org/A182754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=21, a(n)=77*a(n-2) for n≥3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the two explicit initial values. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)21;

    /* Start with 1 for odd n, and 21 for even n. */
    seq_t result = (n & 1) ? (seq_t)1 : (seq_t)21;

    /* Number of times we need to multiply by 77. */
    idx_t mults = (n - ((n & 1) ? 1 : 2)) >> 1;   /* (n-1)/2 for odd, (n-2)/2 for even */

    for (idx_t i = 0; i < mults; ++i)
        result *= (seq_t)77;

    return result;
}

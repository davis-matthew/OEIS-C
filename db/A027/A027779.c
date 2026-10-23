/*
 * A027779  a(n) = 3*(n+1)*binomial(n+2,6).
 * https://oeis.org/A027779
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(n+1)*binomial(n+2,6)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is not defined in OEIS but we
       simply compute the formula; it yields 0 for n < 4. */
    seq_t bin = binomial(n + 2, 6);
    __int128 val = (__int128)bin * (n + 1);
    val *= 3;
    return (seq_t)val;
}

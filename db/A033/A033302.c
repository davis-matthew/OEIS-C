/*
 * A033302  Largest prime < largest prime < n.
 * https://oeis.org/A033302
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime < largest prime < n (i.e. a(n) =
 *            prev_prime(prev_prime(n)))
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;           /* behaviour below the offset is irrelevant */
    idx_t p1 = prev_prime(n);   /* largest prime < n */
    idx_t p2 = prev_prime(p1);  /* largest prime < p1 */
    return (seq_t)p2;
}

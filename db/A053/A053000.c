/*
 * A053000  a(n) = (smallest prime > n^2) - n^2.
 * https://oeis.org/A053000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (smallest prime > n^2) - n^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute n^2 in the wider idx_t type to avoid overflow of seq_t. */
    idx_t sq   = n * n;   /* n^2 */
    idx_t cand = sq + 1;  /* candidate prime */
    while (!is_prime(cand))
    {
        ++cand;
    }
    /* Difference fits in seq_t for the used test range. */
    return (seq_t)(cand - sq);
}

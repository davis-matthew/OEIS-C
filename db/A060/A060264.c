/*
 * A060264  First prime after 2n.
 * https://oeis.org/A060264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least prime greater than 2*n
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Special case for n = 0: the first prime after 0 is 2. */
    if (n == 0) return (seq_t)2;

    /* Compute 2*n; use idx_t to avoid overflow for the given n. */
    idx_t two_n = n << 1;          /* 2*n */

    /* First candidate greater than 2*n. */
    idx_t cand = two_n + 1;

    /* Ensure cand is odd (all primes > 2 are odd). */
    if ((cand & 1) == 0) cand++;

    /* Search forward until a prime is found. */
    while (!is_prime(cand))
    {
        cand += 2;   /* skip even numbers */
    }

    return (seq_t)cand;
}

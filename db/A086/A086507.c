/*
 * A086507  If n is even, a(n) = smallest prime == 1 (mod n), If n is odd,
 *          a(n) = smallest prime == -1 (mod n).
 * https://oeis.org/A086507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p > 1 such that p ≡ 1 (mod n) if n is
 *            even, and p ≡ -1 (mod n) if n is odd
 * Range:     u8 n <= 42, u16 n <= 1666, u32 n <= 7707, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset */
    /* Determine residue r */
    seq_t r = ((n & 1) == 0) ? (seq_t)1 : (seq_t)(n - 1);

    seq_t p   = r;                /* candidate prime */
    seq_t step = (seq_t)n;        /* amount to add each step */

    /* Ensure candidate is at least 2 */
    while (p < 2)
        p += step;

    /* Search for the first prime congruent to required residue */
    while (!is_prime_big(p))
        p += step;

    return p;
}

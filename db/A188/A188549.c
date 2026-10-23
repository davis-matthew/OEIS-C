/*
 * A188549  Numbers k such that 8*k^2+1 is a prime.
 * https://oeis.org/A188549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 12, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n‑th integer k such that 8*k^2+1 is prime. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* undefined for n ≤ 0 */

    idx_t found = 0;
    idx_t k = 1;

    while (found < n)
    {
        /* Compute 8*k^2+1 as a seq_t. */
        seq_t val = ((seq_t)8 * (seq_t)k * (seq_t)k) + (seq_t)1;

        if (is_prime_big(val))
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }

    /* Unreachable return (satisfies the compiler). */
    return (seq_t)0;
}

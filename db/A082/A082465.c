/*
 * A082465  Least k>=1 such that n^2+kn+1 is prime.
 * https://oeis.org/A082465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k ≥ 1 such that n² + k·n + 1 is prime
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Assumes n ≥ 1; behaviour for n = 0 is irrelevant. */
    for (idx_t k = 1;; ++k)
    {
        /* Use 128‑bit arithmetic to avoid overflow of the intermediate value. */
        unsigned __int128 val128 = (unsigned __int128)n * ((unsigned __int128)n + k) + 1;

        /* The value is guaranteed to fit in idx_t for the range of n we handle. */
        if (is_prime((idx_t)val128))
            return (seq_t)k;
    }
}

/*
 * A155723  Numbers k such that 2*k + 9 is not prime.
 * https://oeis.org/A155723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th non‑negative integer k for which 2*k+9 is not
 *            prime
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        if (!is_prime(2 * k + 9))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}

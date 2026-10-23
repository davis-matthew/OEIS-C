/*
 * A025583  Composite numbers that are not the sum of 2 primes.
 * https://oeis.org/A025583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025583 – Composite numbers that are not the sum of 2 primes.
   Computation: iterate odd composite numbers k (starting at 27) and select
   those where (k-2) is also composite. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, but safe */
    idx_t count = 0;
    seq_t candidate = (seq_t)27;
    seq_t two = (seq_t)2;
    for (;;)
    {
        if (!is_prime_big(candidate))
        {
            if (!is_prime_big(candidate - two))
            {
                ++count;
                if (count == n) return candidate;
            }
        }
        candidate += (seq_t)2;
    }
}

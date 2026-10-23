/*
 * A140847  Primes of the form 210k + 37.
 * https://oeis.org/A140847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form 210k + 37 (k ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 131, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;

    while (count < n)
    {
        /* candidate = 210*k + 37 */
        seq_t candidate = (seq_t)((seq_t)210 * (seq_t)k + (seq_t)37);

        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }

    /* unreachable for valid n ≥ 1, keep compiler happy */
    return (seq_t)0;
}

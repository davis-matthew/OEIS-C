/*
 * A087554  a(n) = smallest number k >= n such that nk + 1 is a prime.
 * https://oeis.org/A087554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ n such that n*k + 1 is prime
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* All operations use idx_t (uint64_t) to avoid overflow. */
    idx_t k = n;
    for (;; ++k)
    {
        idx_t val = n * k + 1;          /* compute n*k+1 */
        if (is_prime(val))
            return (seq_t)k;           /* cast back to seq_t for the result */
    }
}

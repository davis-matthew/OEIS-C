/*
 * A082519  Number of times that k^2 + (n-k)^2 is prime for 1 <= k <= n-1.
 * https://oeis.org/A082519
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k | 1 ≤ k ≤ n-1 and k² + (n-k)² is prime }|
 * Range:     u8 n <= 894, u16 n <= 9440, u32 n <= 9440, u64 n <= 9440,
 *            u128 n <= 9440, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; for smaller indices return 0. */
    if (n < 2)
        return 0;

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        idx_t nk   = n - k;
        idx_t term = k * k + nk * nk;   /* value to test for primality */
        if (isPrime(term))
            ++count;
    }
    return count;
}

/*
 * A057458  Number of k, 1 <= k <= n, where {k (n+1-k) + 1} is prime.
 * https://oeis.org/A057458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of k, 1 ≤ k ≤ n, for which k*(n+1‑k)+1 is
 *            prime
 * Range:     u8 n <= 30, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute k*(n+1-k)+1 */
        seq_t prod = (seq_t)k * (seq_t)(n + 1 - k);
        seq_t m = prod + 1;
        /* test primality; the result fits in idx_t */
        if (is_prime((idx_t)m))
            ++count;
    }
    return count;
}

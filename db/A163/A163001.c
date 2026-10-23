/*
 * A163001  a(n) = the number of integers k, 1 <= k <= n-1, such that
 *          prime(n)-prime(k) is divisible by n-k (where prime(n) = the n-th
 *          prime.)
 * https://oeis.org/A163001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : 1 <= k < n, (prime(n)-prime(k)) % (n-k) == 0 }|
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Computes the n-th term of OEIS sequence A163001 */
seq_t a(idx_t n)
{
    if (n <= 1u) return 0;

    idx_t p_n = (idx_t)get_prime(n);
    seq_t count = 0;

    for (idx_t k = 1; k < n; ++k)
    {
        idx_t p_k   = (idx_t)get_prime(k);
        idx_t diff  = p_n - p_k;
        idx_t denom = n - k;

        if (diff % denom == 0)
            ++count;
    }

    return count;
}

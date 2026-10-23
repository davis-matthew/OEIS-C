/*
 * A026728  a(n) = number of primes of the form k*(n-k) + 1.
 * https://oeis.org/A026728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes of the form k*(n-k)+1 with 1 ≤
 *            k ≤ n-1. Because k and n‑k give the same value, it suffices to
 *            examine k = 1 … ⌊n/2⌋
 * Range:     u8 n <= 31, u16 n <= 1058, u32 n <= 1058, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* For n = 0 or 1 the loop is never entered, returning 0. */
    for (idx_t k = 1; k <= n / 2; ++k)
    {
        seq_t val = (seq_t)k * (seq_t)(n - k) + 1;
        if (is_prime_big(val))
            ++count;
    }
    return count;
}

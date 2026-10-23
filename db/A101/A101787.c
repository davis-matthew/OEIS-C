/*
 * A101787  a(n) = |S(n)| where S(n) = {i : 1 <= i <= n and 4*i-1 and 8*i-1
 *          are primes}.
 * https://oeis.org/A101787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of i (1 ≤ i ≤ n) such that (4*i-1) and (8*i-1) are
 *            prime
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        uint64_t p1 = 4ULL * (uint64_t)i - 1ULL;
        uint64_t p2 = 8ULL * (uint64_t)i - 1ULL;
        if (is_prime_uint64(p1) && is_prime_uint64(p2))
            count++;
    }
    return count;
}

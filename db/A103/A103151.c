/*
 * A103151  Number of decompositions of 2n+1 into 2p+q, where p and q are both
 *          odd primes (A065091).
 * https://oeis.org/A103151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations 2n+1 = 2p + q where p and q are
 *            odd primes (OEIS A103151)
 * Range:     u8 n <= 440, u16 n <= 10777, u32 n <= 10777, u64 n <= 10777,
 *            u128 n <= 10777, big any n
 * Validated: standard, pass, 111 terms through n = 9378 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* count representations 2n+1 = 2p + q   with odd primes p,q */
    seq_t count = 0;
    seq_t m = 2 * n + 1;            /* 2n+1 */

    /* p must be at most n-1 for q = m - 2p ≥ 3 */
    for (idx_t p = 3; p <= n - 1; p += 2)
    {
        if (!is_prime_u64((uint64_t)p))
            continue;

        seq_t q = m - 2 * (seq_t)p;
        if (q < 3)
            continue;

        if (is_prime_u64((uint64_t)q))
            ++count;
    }

    return count;
}

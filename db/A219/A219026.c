/*
 * A219026  Number of primes p<=n such that 2n-p and 2n+p-2 are both prime.
 * https://oeis.org/A219026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p ≤ n : p, 2n–p, 2n+p–2 are prime}
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 116 terms through n = 9322 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t two_n = 2 * n;

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime(p))
            continue;
        if (!is_prime(two_n - p))
            continue;
        if (!is_prime(two_n + p - 2))
            continue;
        ++count;
    }

    return count;
}

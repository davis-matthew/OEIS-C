/*
 * A071829  Smallest s > 0 such that gpf(n) = gpf(n+s) where gpf = A006530
 *          (greatest prime factor).
 * https://oeis.org/A071829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest s > 0 such that gpf(n) = gpf(n+s)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 79 (2026-11-01)
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* undefined for n < 2 */

    uint64_t N  = (uint64_t)n;
    uint64_t gp = largest_prime_factor(N);            /* greatest prime factor of n */
    uint64_t s  = 1;

    while (true) {
        uint64_t cand = N + s;
        if (largest_prime_factor(cand) == gp)
            break;
        ++s;
    }
    return (seq_t)s;
}

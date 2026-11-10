/*
 * A064923  a(n) = Min { k | A064920(k) = n }.
 * https://oeis.org/A064923
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min { k | A064920(k) = n }, found by search: k = m*p with
 *            p prime, m = n+1-p >= 1, gpf(m) <= p; take the smallest m*p
 * Range:     u8 n <= 31, u16 n <= 521, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 990 (2026-11-07)
 * Helpers:   is_prime, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    bool found = false;
    seq_t best = 0;

    for (uint64_t p = 2; p <= (uint64_t)n; p++) {
        if (!is_prime((idx_t)p))
            continue;
        uint64_t m = (uint64_t)n + 1 - p;
        if (m < 1)
            break;
        if (m > 1 && largest_prime_factor(m) > p)
            continue;
        seq_t k = (seq_t)m * (seq_t)p;
        if (!found || k < best) {
            best = k;
            found = true;
        }
    }
    return best;
}

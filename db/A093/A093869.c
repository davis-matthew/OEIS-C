/*
 * A093869  Frequency of occurrence of prime(n) in A093868.
 * https://oeis.org/A093869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = number of m >= 1 with A093868(m) = prime(n). Such m must
 *            divide prime(n)-1 or prime(n)+1, so count those divisors d (each
 *            once) whose smallest prime of the form d*k+-1 equals prime(n)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-07)
 * Helpers:   is_prime_u64, nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/nth_prime.h"

/* smallest prime of form d*k-1 or d*k+1, searching only up to limit;
   returns 0 if none <= limit */
static uint64_t A093869_smallest(uint64_t d, uint64_t limit)
{
    for (uint64_t k = 1; ; ++k) {
        uint64_t lo = d * k - 1;
        if (lo > limit) return 0;
        if (lo >= 2 && is_prime_u64(lo)) return lo;
        uint64_t hi = d * k + 1;
        if (hi > limit) return 0;
        if (is_prime_u64(hi)) return hi;
    }
}

static uint64_t A093869_count(uint64_t p, uint64_t m, bool second)
{
    uint64_t cnt = 0;
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i) continue;
        uint64_t d1 = i, d2 = m / i;
        if (!(second && ((p - 1) % d1 == 0)))
            if (A093869_smallest(d1, p) == p) ++cnt;
        if (d2 != d1 && !(second && ((p - 1) % d2 == 0)))
            if (A093869_smallest(d2, p) == p) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t p = nth_prime(n);
    uint64_t c = A093869_count(p, p - 1, false) + A093869_count(p, p + 1, true);
    return (seq_t)c;
}

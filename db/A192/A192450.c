/*
 * A192450  Numbers k such that -1 is not a square mod k.
 * https://oeis.org/A192450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 8884 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192450: numbers k such that -1 is not a square mod k.
   A term is included if k is divisible by 4 or if k has a prime factor
   congruent to 3 (mod 4).  The function enumerates k successively
   starting at 3 and returns the nth such number (offset 1). */
#include <stdint.h>
#include <stdbool.h>

/* Return true iff the integer k has a prime factor ≡ 3 (mod 4). */
static bool has_p3mod4(uint64_t k)
{
    /* Trial division by all integers up to sqrt(k). */
    uint64_t t = k;
    if (t % 2ULL == 0ULL) {          /* handle factor 2 separately */
        t /= 2ULL;
        while (t % 2ULL == 0ULL) t /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= t; d += 2ULL) {
        if (t % d == 0ULL) {
            if (d % 4ULL == 3ULL) return true;
            do t /= d; while (t % d == 0ULL);
        }
    }
    /* If a prime factor > sqrt(original k) remains. */
    return (t > 1ULL && t % 4ULL == 3ULL);
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t k = 3ULL; ; ++k) {
        if ((k & 3ULL) == 0ULL || has_p3mod4(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}

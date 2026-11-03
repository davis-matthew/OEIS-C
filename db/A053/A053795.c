/*
 * A053795  Composite numbers ending in 1, 3, 7 or 9.
 * https://oeis.org/A053795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9526 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053795: composite numbers not divisible by 2 or 5,
   ending in 1, 3, 7 or 9.  The function iterates over candidates
   and counts those that are composite and satisfy the digit test. */

#include <stdbool.h>
#include "../utils/is_prime.h"

static inline bool ends_allowed(uint64_t x)
{
    uint8_t d = x % 10;
    return d == 1 || d == 3 || d == 7 || d == 9;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 */

    uint64_t cand = 9;      /* first candidate ending in 9 */
    idx_t   count = 0;

    for (;; ++cand) {
        if (!ends_allowed(cand))
            continue;

        if (!is_prime(cand)) {          /* is_prime(n) == true iff n is prime */
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}

/*
 * A053795  Composite numbers ending in 1, 3, 7 or 9.
 * https://oeis.org/A053795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ends_allowed, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053795: composite numbers not divisible by 2 or 5,
   ending in 1, 3, 7 or 9.  The function iterates over candidates
   and counts those that are composite and satisfy the digit test. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ends_allowed.h"

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

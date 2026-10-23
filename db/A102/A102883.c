/*
 * A102883  Terms which share a divisor greater than 1 with the value formed
 *          by adding them to the sum of their divisors less than or equal to
 *          their square roots.
 * https://oeis.org/A102883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A102883 – Terms which share a divisor > 1 with the value formed by
 *  adding the number to the sum of its divisors ≤ its square root.
 *  For a given index n ≥ 1 this function scans integers m starting
 *  at 1, determines whether m satisfies the property, and returns the
 *  n‑th such m.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd64.h"

/*  Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;              /* how many qualifying integers seen */

    for (idx_t m = 1; ; ++m) {
        /* floor(sqrt(m)) – looped enough for 64‑bit values */
        idx_t limit = 0;
        while ((limit + 1) * (limit + 1) <= m) ++limit;

        /* sum of divisors of m that are ≤ sqrt(m) */
        uint64_t sum = 0;
        for (idx_t d = 1; d <= limit; ++d) {
            if (m % d == 0)
                sum += (uint64_t)d;
        }

        uint64_t s = sum + (uint64_t)m;          /* m + small divisors */
        uint64_t g = gcd64((uint64_t)m, s);      /* gcd(m, s) */

        if (g > 1) {                              /* qualifies */
            ++found;
            if (found == n)
                return (seq_t)m;                 /* cast to output type */
        }
    }
}

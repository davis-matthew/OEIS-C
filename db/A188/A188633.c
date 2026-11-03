/*
 * A188633  Numbers of the form 2^k * m, with k > 1 and m an odd composite
 *          number.
 * https://oeis.org/A188633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9510 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A188633.
   Numbers of the form 2^k * m, with k > 1 and m an odd composite number.
   This implementation iterates over integers, tests the condition,
   and returns the n-th qualifying number. */

#include <stdbool.h>
#include <stdint.h>

/* Test whether an odd integer x is composite.
   Assumes x fits in a 64‑bit unsigned integer (true for all numbers
   encountered when n ≤ 10000). */
static bool is_odd_composite(seq_t x)
{
    uint64_t ux = (uint64_t)x;
    if (ux <= 1) return false;           /* 1 and 0 are not composite */
    if ((ux & 1ULL) == 0) return false;  /* even numbers are not considered */

    /* trial division by odd divisors up to sqrt(ux) */
    for (uint64_t d = 3; d * d <= ux; d += 2)
        if (ux % d == 0) return true;    /* found a factor */
    return false;                        /* no factor found → prime */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against invalid input */

    idx_t count = 0;    /* number of terms found so far */
    seq_t candidate = 4; /* start from the smallest multiple of 4 */

    for (;; ++candidate)
    {
        /* candidate must be divisible by 4 (k > 1) */
        if ((candidate & 3) != 0) continue;

        /* strip all factors of 2 to obtain the odd part m */
        seq_t m = candidate;
        while ((m & 1) == 0) m >>= 1;

        /* m must be odd composite */
        if (m > 1 && is_odd_composite(m))
        {
            ++count;
            if (count == n) return candidate;
        }
    }
}

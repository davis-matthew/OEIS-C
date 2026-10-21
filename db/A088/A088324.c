/*
 * A088324  Smallest prime formed by a substring of the decimal digits of 1/n
 *          beginning with the first (nonzero) digit after the decimal point,
 *          or 0 if no such prime exists.
 * https://oeis.org/A088324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime that starts at the first non‑zero decimal
 *            digit of 1/n; returns 0 if no such prime exists
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n <= 1)                /* 1/1 has no fractional part */
        return 0;

    /* Current remainder of 1/n when extracting digits */
    uint64_t rem = 1 % (uint64_t)n;   /* 1 for all n>1 */
    bool started = false;           /* first non‑zero digit seen? */
    seq_t cur = 0;                  /* accumulated number */

    /* Upper bound on the number of digits examined.
       For a terminating decimal rem becomes 0 early.
       For a repeating decimal the period is at most n‑1,
       so 2*n iterations safely cover all possible prefixes.   */
    uint64_t max_digits = (n == 1) ? 0 : 2 * n;

    for (uint64_t i = 0; i < max_digits && rem != 0; ++i) {
        /* Generate next decimal digit of 1/n */
        uint64_t d = (rem * 10) / (uint64_t)n;
        rem     = (rem * 10) % (uint64_t)n;

        if (!started) {
            if (d == 0)         /* skip leading zeros */
                continue;
            started = true;     /* first non‑zero digit reached */
        }

        /* Append digit to the current number */
        cur = cur * 10 + d;

        /* Test for primality */
        if (is_prime_big(cur))
            return cur;
    }

    /* No prime found within the examined digits */
    return 0;
}

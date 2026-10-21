/*
 * A083116  Smallest multiple of n using a single digit with multiplicity, or
 *          0 if no such number exists.
 * https://oeis.org/A083116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest repdigit (a positive integer consisting of
 *            one digit repeated) that is a multiple of n; if no such number
 *            exists, a(n) = 0
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Find the smallest multiple of n that is a repdigit (consisting of a single digit repeated).  Return 0 if none exists. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t best = 0;
    bool has_best = false;

    for (int d = 1; d <= 9; ++d) {
        seq_t val = d;
        idx_t rem = (idx_t)(d % n);

        /* If the single digit already works, record it. */
        if (rem == 0) {
            if (!has_best || val < best) {
                best = val;
                has_best = true;
            }
            continue;
        }

        /* Generate longer repdigits of the same digit and look for a divisor. */
        for (idx_t step = 1; step < n; ++step) {
            /* Compute the new remainder: (rem*10 + d) mod n.
               Use 128‑bit arithmetic to avoid overflow on 64‑bit builds. */
            __uint128_t t = (__uint128_t)rem * 10 + d;
            rem = (idx_t)(t % n);

            /* Update the numeric value of the repdigit. */
            val = val * 10 + d;

            if (rem == 0) {
                if (!has_best || val < best) {
                    best = val;
                    has_best = true;
                }
                break; /* Found the shortest repdigit for this digit. */
            }
        }
    }

    return has_best ? best : 0;
}

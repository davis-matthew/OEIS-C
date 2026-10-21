/*
 * A055240  Number of bases in which n is not divisible by any of its digits.
 * https://oeis.org/A055240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = #{ b | 2 ≤ b < n : for every non‑zero digit d of n in
 *            base b, n mod d ≠ 0 }
 * Range:     u8 n <= 556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* seq_t and idx_t are supplied by the build.  No other includes are needed. */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Bases are considered only up to n‑1, because base n gives the
       representation “10” and digit 1 divides n.  For n ≤ 2 the loop
       executes zero times. */
    for (idx_t b = 2; b < n; ++b) {
        idx_t temp = n;
        bool ok = true;

        /* Extract digits of n in base b.  Zero digits do not affect the
           condition.  If any non‑zero digit divides n, the base fails. */
        while (temp > 0) {
            idx_t d = temp % b;
            if (d != 0) {
                if (n % d == 0) {
                    ok = false;
                    break;
                }
            }
            temp /= b;
        }

        if (ok) {
            ++count;
        }
    }

    return count;
}

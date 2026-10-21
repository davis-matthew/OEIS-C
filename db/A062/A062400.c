/*
 * A062400  Smallest multiple of n with property that digits are even and each
 *          digit is two more (mod 10) than the previous digit; or 0 if no
 *          such multiple exists.
 * https://oeis.org/A062400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest multiple of n that has decimal digits all
 *            even and each digit is two more (mod 10) than the previous one
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 20, u64 n <= 42, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const int starts[4] = {2, 4, 6, 8};
    bool found = false;
    seq_t best = 0;

    uint64_t nn = (uint64_t)n;          /* used for the remainder calculations */

    /* For each possible starting digit (2,4,6,8) */
    for (int s = 0; s < 4; ++s)
    {
        int digit = starts[s];
        uint64_t rem = (uint64_t)digit % nn;
        seq_t   val = digit;

        /* Single‑digit candidate */
        if (rem == 0) {
            if (!found || val < best) { best = val; found = true; }
            continue;
        }

        /* Iteratively append digits.  After 5 * nn steps all states
           (rem,position_in_cycle) have been visited; if no zero
           remainder occurs then none exists for this starting digit. */
        idx_t limit = (idx_t)((unsigned __int128)5 * nn);
        for (idx_t i = 0; i < limit; ++i)
        {
            digit = (digit + 2) % 10;
            rem   = (rem * 10 + digit) % nn;
            val   = val * 10 + digit;

            if (rem == 0) {
                if (!found || val < best) { best = val; found = true; }
                break;                          /* minimal for this start */
            }
        }
    }

    return found ? best : 0;
}

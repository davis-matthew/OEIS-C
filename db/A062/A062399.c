/*
 * A062399  Smallest multiple of n with property that each digit is one less
 *          (mod 10) than the previous digit; or 0 if no such multiple exists.
 * https://oeis.org/A062399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest multiple of n such that each decimal digit is
 *            one less (mod 10) than the previous digit; 0 if no such multiple
 *            exists
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 22, u64 n <= 92, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)      /* a(0) = 0 by definition */
        return 0;

    seq_t best = 0;            /* 0 denotes "no candidate found yet" */

    for (idx_t d = 1; d <= 9; ++d)   /* starting digit cannot be 0 for n > 0 */
    {
        seq_t rem   = (seq_t)d % (seq_t)n;
        seq_t value = (seq_t)d;

        if (rem == 0)            /* single‑digit multiple */
        {
            if (best == 0 || value < best)
                best = value;
            continue;
        }

        /* Build the decreasing‑digit number by appending one digit at a time.
           After k digits have been added (k≥1), the next digit is
           (d - k) mod 10.  We only need n steps; if we never hit remainder 0
           within those steps, no such multiple exists for this starting digit. */
        for (idx_t k = 1; k < n; ++k)
        {
            int r     = (int)(k % 10); /* position modulo 10 */
            int digit = (d + 10 - r) % 10;   /* (d - k) mod 10 */

            rem   = (rem * 10 + (seq_t)digit) % (seq_t)n;
            value = value * 10 + (seq_t)digit;

            if (rem == 0)                  /* found a multiple */
            {
                if (best == 0 || value < best)
                    best = value;
                break;                     /* stop extending this starting digit */
            }
        }
    }

    return best;          /* 0 if none was found */
}

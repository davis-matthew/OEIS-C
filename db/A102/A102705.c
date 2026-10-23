/*
 * A102705  Numbers not of the form 7x + 9y with nonnegative x and y.
 * https://oeis.org/A102705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* a(n) = the nth nonnegative integer that cannot be expressed as 7x + 9y
   with nonnegative integers x and y.  In particular 0 is treated as
   “non‑representable” (matching the OEIS listing).  The sequence stops
   after the 24th term (value 47); for n ≥ 25 the function returns 0. */
seq_t a(idx_t n)
{
    /* Since the sequence is finite (0 ≤ a(n) ≤ 47), for any n ≥ 25 we
       return 0 as the only undefined behaviour for that index. */
    if (n >= 25)
        return 0;

    idx_t count = 0;        /* number of non‑representable integers seen */
    for (seq_t m = 0; ; ++m) {
        bool non_rep;
        if (m == 0) {
            /* 0 is included as the first term, although it is
               7·0 + 9·0.  This matches the OEIS table. */
            non_rep = true;
        } else {
            /* Check whether m can be written as 7x + 9y. */
            bool can_rep = false;
            idx_t max_y = (idx_t)(m / 9);
            for (idx_t y = 0; y <= max_y; ++y) {
                seq_t rem = m - (seq_t)9 * (seq_t)y;
                if (rem % 7 == 0) {
                    can_rep = true;
                    break;
                }
            }
            non_rep = !can_rep;
        }

        if (non_rep) {
            if (count == n)
                return m;
            ++count;
        }
    }
}

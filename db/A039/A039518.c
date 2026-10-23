/*
 * A039518  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 7's.
 * https://oeis.org/A039518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039518 – numbers whose base‑12 representation has
 * the same nonzero number of 2's and 7's. Computed by a brute‑force
 * search over successive integers. */

#include <stdlib.h>

/* Compute the nth term (1‑based index). */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of valid numbers found so far   */
    seq_t candidate = 0;      /* current integer being tested          */

    while (count < n)
    {
        candidate++;

        seq_t t = candidate;
        unsigned cnt2 = 0, cnt7 = 0;

        /* Count 2's and 7's in the base‑12 representation. */
        while (t > 0)
        {
            seq_t d = t % 12;
            if (d == 2)      cnt2++;
            else if (d == 7) cnt7++;
            t /= 12;
        }

        if (cnt2 == cnt7 && cnt2 > 0)
            count++;
    }

    return candidate;
}

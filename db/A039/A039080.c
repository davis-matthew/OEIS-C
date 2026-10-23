/*
 * A039080  Numbers whose base-9 representation has the same number of 0's and
 *          4's.
 * https://oeis.org/A039080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer whose base‑9 representation
 * contains an equal number of 0's and 4's. This is OEIS A039080. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t x = 1; ; ++x)
    {
        // Count 0's and 4's in base‑9 representation of x.
        seq_t val = (seq_t)x;
        seq_t tmp = val;
        unsigned cnt0 = 0, cnt4 = 0;
        while (tmp > (seq_t)0)
        {
            seq_t digit = tmp % (seq_t)9;
            if (digit == (seq_t)0) ++cnt0;
            else if (digit == (seq_t)4) ++cnt4;
            tmp /= (seq_t)9;
        }
        // Numbers without any 0 or 4 also qualify (cnt0 == cnt4 == 0).
        if (cnt0 == cnt4)
        {
            ++found;
            if (found == n)
                return (seq_t)x;
        }
    }
}

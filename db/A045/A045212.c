/*
 * A045212  Numbers whose base-5 representation contains exactly two 0's and
 *          two 4's.
 * https://oeis.org/A045212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A045212: brute‑force search for numbers whose base‑5 representation
   contains exactly two 0's and two 4's */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t cand = 0; ; ++cand)
    {
        seq_t tmp = cand;
        unsigned zeros = 0, fours = 0;

        if (tmp == 0)              /* representation "0" */
            zeros = 1;
        else
            while (tmp > 0)
            {
                unsigned d = (unsigned)(tmp % 5);
                if (d == 0) ++zeros;
                else if (d == 4) ++fours;
                tmp /= 5;
            }

        if (zeros == 2 && fours == 2)
        {
            ++count;
            if (count == n)
                return cand;
        }
    }
}

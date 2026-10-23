/*
 * A039207  Numbers whose base-11 representation has the same number of 8's
 *          and 9's.
 * https://oeis.org/A039207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer whose base‑11
 *            representation contains equal numbers of the digits 8 and 9
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t val = 0; ; ++val)
    {
        unsigned count8 = 0, count9 = 0;
        seq_t tmp = val;
        while (tmp > 0)
        {
            unsigned d = (unsigned)(tmp % 11);
            if (d == 8)  ++count8;
            else if (d == 9) ++count9;
            tmp /= 11;
        }
        if (count8 == count9)
        {
            ++found;
            if (found == n) return val;
        }
    }
}

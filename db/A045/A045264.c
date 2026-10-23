/*
 * A045264  Numbers whose base-5 representation contains no 2's and exactly
 *          two 3's.
 * https://oeis.org/A045264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration of base‑5 numbers
   with no digit 2 and exactly two digits 3. */
#include <stdbool.h>

static bool is_valid(seq_t x)
{
    int count3 = 0;
    while (x > 0)
    {
        seq_t d = x % 5;
        if (d == 2)
            return false;
        if (d == 3)
        {
            count3++;
            if (count3 > 2)
                return false;
        }
        x /= 5;
    }
    return count3 == 2;
}

seq_t a(idx_t n)
{
    seq_t x = 0;
    idx_t found = 0;
    while (found < n)
    {
        x++;
        if (is_valid(x))
            found++;
    }
    return x;
}

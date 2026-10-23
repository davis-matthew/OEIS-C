/*
 * A039408  Numbers whose base-10 representation has the same nonzero number
 *          of 1's and 8's.
 * https://oeis.org/A039408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose decimal representation
 *            contains an equal nonzero number of 1's and 8's
 * Range:     u8 n <= 19, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* The sequence function.  The type definitions for seq_t and idx_t are
   supplied by the build.  Do not change the signature. */
seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of qualifying numbers seen */
    seq_t num = 1;            /* current integer being tested */
    
    while (1)
    {
        int c1 = 0, c8 = 0;
        seq_t x = num;
        /* count digits 1 and 8 */
        while (x > 0)
        {
            int d = (int)(x % 10);
            if (d == 1)  c1++;
            else if (d == 8) c8++;
            x /= 10;
        }
        if (c1 == c8 && c1 > 0)          /* qualifies? */
        {
            found++;
            if (found == n)
                return num;
        }
        num++;
    }
}

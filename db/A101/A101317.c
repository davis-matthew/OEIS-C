/*
 * A101317  Numbers n with >= 3 digits such that sum of external digits = sum
 *          of internal digits.
 * https://oeis.org/A101317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n with >= 3 digits such that sum of external digits = sum of internal digits.
   Implements a(n) by brute‑force search: iterate over natural numbers >= 100,
   test the condition, and return the nth one. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1; 0 is undefined */

    idx_t found = 0;
    seq_t num = 100;                  /* start with the smallest 3‑digit number */

    while (true)
    {
        seq_t x = num;
        seq_t last = x % 10;           /* most right digit */
        x /= 10;

        seq_t internal = 0;
        /* Sum all middle digits (remove first digit afterwards) */
        while (x >= 10)                /* keep at least two digits left */
        {
            internal += x % 10;
            x /= 10;
        }
        seq_t first = x;                /* remaining leftmost digit */

        if (first + last == internal)
        {
            ++found;
            if (found == n)
                return num;             /* nth valid number found */
        }
        ++num;                          /* check next integer */
    }
}

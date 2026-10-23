/*
 * A121025  Multiples of 5 containing a 5 in their decimal representation.
 * https://oeis.org/A121025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 9058, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate multiples of 5 and select those that contain the digit 5 */

#include <stdbool.h>

/* Helper: check if a number contains the decimal digit 5 */
static bool contains_digit5(seq_t x)
{
    while (x > 0)
    {
        if ((x % 10) == 5)
            return true;
        x /= 10;
    }
    return false;
}

/* Return the nth positive integer that is a multiple of 5 and contains digit 5 */
seq_t a(idx_t n)
{
    idx_t k = 1;      /* multiplier for 5 */
    idx_t cnt = 0;    /* number of qualifying terms found so far */

    while (cnt < n)
    {
        seq_t val = (seq_t)5 * (seq_t)k;   /* current multiple of 5 */
        if (contains_digit5(val))
        {
            ++cnt;
            if (cnt == n)
                return val;
        }
        ++k;
    }

    /* The loop always returns; this line is for completeness */
    return (seq_t)0;
}

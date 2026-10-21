/*
 * A222247  In the number n, replace all (decimal) digits '5' with '8' and
 *          vice versa.
 * https://oeis.org/A222247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = replace each decimal digit 5 by 8 and each digit 8 by 5
 * in the decimal representation of n.
 *
 * This implements the OEIS entry A222247.  It works for any unsigned
 * index n (idx_t) and returns a unsigned value of type seq_t.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;    /* accumulated swapped number */
    seq_t mult   = 1;    /* current place value multiplier */

    /* Process each decimal digit of n from least significant to most */
    while (n > 0) {
        idx_t digit = n % 10;          /* get rightmost digit */
        if (digit == 5) {
            digit = 8;
        } else if (digit == 8) {
            digit = 5;
        }
        result += (seq_t)digit * mult; /* add modified digit */
        mult   *= 10;                  /* shift to next place value */
        n      /= 10;                  /* remove processed digit */
    }

    return result;
}

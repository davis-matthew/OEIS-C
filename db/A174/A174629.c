/*
 * A174629  "Binary dates" take the dates with the format mm/dd/yy that have
 *          only 0's and 1's and transform their value to decimal base.
 * https://oeis.org/A174629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = 16*( (( (n-1)%9 )/3)+1 ) * + 4*( ( (n-1)%3 ) +1 ) * +
 *            ((n-1)/9)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The expression interprets the binary date mm/dd/yy (with
   month ∈{01,10,11}, day ∈{01,10,11}, year ∈{00,01,10,11})
   as a 6‑bit binary number and returns its decimal value.
   The order follows: year, month, day.                                            */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1.  For n > 36 we simply return 0 (undefined). */
    idx_t n1   = n - 1;
    idx_t year = n1 / 9;
    idx_t rem  = n1 % 9;
    idx_t month = rem / 3 + 1;   /* month = 1,2,3 */
    idx_t day   = rem % 3 + 1;   /* day   = 1,2,3 */

    return (seq_t)month * (seq_t)16 + (seq_t)day * (seq_t)4 + (seq_t)year;
}

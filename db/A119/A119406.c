/*
 * A119406  Years in which there are five Sundays in the month of February.
 * https://oeis.org/A119406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th year where February has five Sundays.
   Those years are the leap years whose February 1st falls on Sunday. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1 */
    idx_t year = 1756;                 /* first term of the sequence */
    seq_t dow = 0;                     /* Sunday for Feb 1 1756 */
    idx_t count = 0;

    while (1) {
        bool leap = ((year % 4 == 0) &&
                     (year % 100 != 0 || year % 400 == 0));
        if (leap && dow == 0) {        /* 5 Sundays in February */
            count++;
            if (count == n)
                return (seq_t)year;
        }
        /* advance to the next year */
        int shift = leap ? 2 : 1;       /* days to next Feb 1 */
        dow = (dow + (seq_t)shift) % 7;
        year++;
    }
}

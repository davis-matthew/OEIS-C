/*
 * A127252  Sequence composed of 1 and -1 with the -1's occurring at odious
 *          indexed positions given by A091855.
 * https://oeis.org/A127252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = -1 if popcount(n) is odd and the number of trailing zeros
 *            * in n is even; otherwise a(n) = 1
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 is a special case (even popcount, no trailing zeros). */
    if (n == 0)
        return (seq_t)1;

    /* Parity of popcount(n).  parity==0 => even, parity==1 => odd. */
    int parity = 0;
    idx_t tmp = n;
    while (tmp) {
        parity ^= 1;          /* flip parity each time a 1 bit is removed */
        tmp &= tmp - 1;        /* remove the lowest set bit */
    }
    if (parity == 0)              /* even popcount → 1 */
        return (seq_t)1;

    /* Parity of the number of trailing zeros.  tz_parity==0 means
     * an even number of trailing zeros, tz_parity==1 means odd. */
    int tz_parity = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) {  /* while the lowest bit is zero */
        tz_parity ^= 1;            /* flip parity each time we shift */
        m >>= 1;
    }

    /* Return -1 if popcount odd *and* trailing zeros even, else 1. */
    if (tz_parity == 0)
        return (seq_t)-1;
    else
        return (seq_t)1;
}

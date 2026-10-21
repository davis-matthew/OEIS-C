/*
 * A218132  Number of length 9 primitive (=aperiodic or period 9) n-ary words.
 * https://oeis.org/A218132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 - n^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert idx_t to the sequence type */
    seq_t sn = (seq_t)n;

    /* Compute n^3 */
    seq_t pow3 = sn;
    pow3 *= sn;
    pow3 *= sn;          /* now pow3 == n^3 */

    /* Compute n^9 = (n^3)^3 */
    seq_t pow9 = pow3;
    pow9 *= pow3;
    pow9 *= pow3;        /* now pow9 == n^9 */

    /* Return the difference */
    return pow9 - pow3;
}

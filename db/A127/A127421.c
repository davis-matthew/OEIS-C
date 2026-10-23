/*
 * A127421  Numbers whose decimal expansion is a concatenation of 2
 *          consecutive increasing nonnegative numbers.
 * https://oeis.org/A127421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n‑1)*10^digits(n) + n, i.e. the decimal concatenation of
 *            the consecutive numbers n‑1 and n, with the first term
 *            corresponding to n = 1
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* In OEIS a(1) corresponds to the pair 0,1. */
    idx_t k   = n - 1;      /* first number to concatenate */
    idx_t next= n;          /* second number (k+1) */

    /* Count the decimal digits of the second number.
       Use a simple loop to avoid external utilities.  */
    idx_t d = 1;
    idx_t tmp = next / 10;
    while (tmp > 0) {
        d++;
        tmp /= 10;
    }

    /* Compute 10^d as a seq_t by repeated multiplication.  */
    seq_t ten = (seq_t)10;
    seq_t mult = (seq_t)1;
    for (idx_t i = 0; i < d; ++i)
        mult *= ten;

    /* Concatenate the numbers: k*10^d + next.  */
    return (seq_t)k * mult + (seq_t)next;
}

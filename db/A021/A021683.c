/*
 * A021683  Decimal expansion of 1/679.
 * https://oeis.org/A021683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/679
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) gives the n-th digit after the decimal point of 1/679.
   The digits are computed by repeated long division: each digit is
   (remainder*10)/679, and the new remainder is (remainder*10)%679.
   The algorithm runs in O(n) time, which is fast enough for n up to
   about 10000. */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;                 /* initial remainder for 1/679 */
    for (idx_t i = 0; i < n; ++i)    /* advance to the desired digit */
        rem = (rem * 10ULL) % 679ULL; /* update remainder */

    uint64_t digit = (rem * 10ULL) / 679ULL; /* compute digit */
    return (seq_t)digit;                  /* return as seq_t */
}

/*
 * A021897  Decimal expansion of 1/893.
 * https://oeis.org/A021897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = the nth digit following the decimal point in the decimal
 *            expansion of 1/893. The first term a(0) is the digit immediately
 *            after the decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Multiplication never exceeds 8930, so a 64‑bit integer is
       sufficient for the intermediate remainder. */
    uint64_t rem = 1;          /* remainder of 1 after division by 893 */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem *= 10ULL;                      /* bring down a zero */
        seq_t digit = (seq_t)(rem / 893ULL);/* quotient is the digit   */
        rem %= 893ULL;                     /* new remainder          */
        if (i == n) return digit;          /* return the requested digit */
    }
    /* Unreachable, keeps the compiler happy */
    return 0;
}

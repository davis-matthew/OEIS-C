/*
 * A021618  Decimal expansion of 1/614.
 * https://oeis.org/A021618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 0 ;
 *          a(n) = digit at position n+1 of the fractional part of 1/614
 *                 (i.e. a(n) = (10^(n+1) / 614) % 10   for n >= 1).
 *
 * This implementation generates the digits by long division, performing
 * O(n) operations per call which is fast for n up to about 10 000.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Return the integer part (0) for n = 0 */
    if (n == 0)
        return 0;

    /* Remainder after the integer part; initially 1 / 614 */
    uint64_t rem = 1 % 614;   /* rem < 614 */

    /* Generate digits up to position n+1 (the digit we need) */
    for (idx_t i = 1; i <= n + 1; ++i) {
        rem *= 10;                     /* shift left by one decimal digit   */
        seq_t digit = rem / 614;        /* integer division gives current digit */
        rem    %= 614;                  /* new remainder for next digit      */
        if (i == n + 1)
            return digit;
    }

    /* Should never reach here */
    return 0;
}
